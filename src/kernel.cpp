#include <kernel.h>
#include <uint256.h>
#include <chainparams.h>
#include <util.h>
#include <init.h>
#include <timedata.h>
#include <txdb.h>
#include <validation.h>
#include <index/txindex.h>
#include <chain.h>
#include <script/interpreter.h>
#include <unordered_map>

using namespace std;

struct Hasher
{
    size_t operator()(const uint256& hash) const { return hash.GetCheapHash(); }
};
//blocktime, TxPrevOffset, amount of prevtx, prevout.n
unordered_map<uint256, tuple<uint32_t, unsigned int, CAmount, uint64_t>, Hasher>m_txdata;

// ppcoin kernel protocol
// coinstake must meet hash target according to the protocol:
// kernel (input 0) must meet the formula
//     hash(nStakeModifier + txPrev.block.nTime + txPrev.offset + txPrev.nTime + txPrev.vout.n + nTime) < bnTarget * nCoinDayWeight
// this ensures that the chance of getting a coinstake is proportional to the
// amount of coin age one owns.
// The reason this hash is chosen is the following:
//   nStakeModifier: 
//       (v0.3) scrambles computation to make it very difficult to precompute
//              future proof-of-stake at the time of the coin's confirmation
//       (v0.2) nBits (deprecated): encodes all past block timestamps
//   txPrev.block.nTime: prevent nodes from guessing a good timestamp to
//                       generate transaction for future advantage
//   txPrev.offset: offset of txPrev inside block, to reduce the chance of 
//                  nodes generating coinstake at the same time
//   txPrev.nTime: reduce the chance of nodes generating coinstake at the same
//                 time
//   txPrev.vout.n: output number of txPrev, to reduce the chance of nodes
//                  generating coinstake at the same time
//   block/tx hash should not be used here as they can be generated in vast
//   quantities so as to generate blocks faster, degrading the system back into
//   a proof-of-work situation.
//
bool CheckStakeKernelHash(unsigned int nBits, uint32_t nTimeBlockFrom, unsigned int nTxPrevOffset, CAmount nAmount, uint64_t n, uint32_t nTimeTx, uint256& hashProofOfStake)
{
    if (nTimeBlockFrom + Params().GetConsensus().nStakeMinAge > nTimeTx) // Min age requirement
        return false;
        //return error("CheckStakeKernelHash() : min age violation");

    arith_uint256 bnTargetPerCoinDay;
    bnTargetPerCoinDay.SetCompact(nBits);

    // v0.3 protocol kernel hash weight starts from 0 at the 30-day min age
    // this change increases active coins participating the hash and helps
    // to secure the network when proof-of-stake difficulty is low
    int64_t nTimeWeight = min((int64_t)nTimeTx - nTimeBlockFrom, Params().GetConsensus().nStakeMaxAge) - Params().GetConsensus().nStakeMinAge;

    arith_uint256 bnCoinDayWeight = arith_uint256(nAmount) * nTimeWeight / COIN / (24 * 60 * 60);

    // Calculate hash
    CDataStream ss(SER_GETHASH, 0);

    ss << nBits << nTimeBlockFrom << nTxPrevOffset << nTimeBlockFrom << n << nTimeTx;

    hashProofOfStake = Hash(ss.begin(), ss.end());
    //printf("hash = %s, target = %s\n", hashProofOfStake.ToString().c_str(), ArithToUint256(bnCoinDayWeight * bnTargetPerCoinDay).ToString().c_str());
    // Now check if proof-of-stake hash meets target protocol
    if (UintToArith256(hashProofOfStake) > bnCoinDayWeight * bnTargetPerCoinDay)
        return false;

    return true;
}

bool CheckStakeKernelHash(unsigned int nBits, const CBlock& blockFrom, unsigned int nTxPrevOffset, const CTxOut& txOutPrev, const COutPoint& prevout, uint32_t nTimeTx, uint256& hashProofOfStake)
{
    return CheckStakeKernelHash(nBits, blockFrom.GetBlockTime(),nTxPrevOffset, txOutPrev.nValue, prevout.n, nTimeTx, hashProofOfStake);
}

bool CheckProofOfStake(const CTransactionRef& tx, unsigned int nBits, uint256& hashProofOfStake, unsigned int nBlockTime)
{
    uint256 hash = tx->GetHash();
    auto itr = m_txdata.find(hash);

    uint32_t nTimeBlockFrom;
    unsigned int nTxPrevOffset;
    CAmount nAmount;
    uint64_t n;
    if(itr != m_txdata.end())
    {
        //found
        auto pair = itr->second;
        nTimeBlockFrom = get<0>(pair);
        nTxPrevOffset = get<1>(pair);
        nAmount = get<2>(pair);
        n = get<3>(pair);
    }
    else
    {
        //not found
        const CTxIn& txin = tx->vin[0];

        CTransactionRef txTmp;
        uint256 hash;

        if (!GetTransaction(txin.prevout.hash, txTmp, Params().GetConsensus(), hash, true))
        {
            LogPrintf("CheckProofOfStake() : txPrev not found hash = %s\n", txin.prevout.hash.ToString().c_str());
            return false;
        }
        //return state.DoS(1, error("CheckProofOfStake() : txPrev not found")); // previous transaction not in main chain, may occur during initial download
        // Verify signature
        PrecomputedTransactionData txdata(*tx);
        if (!CScriptCheck(txTmp->vout[tx->vin[0].prevout.n], *tx, 0, 0, true, &txdata)())
        {
            LogPrintf("CheckProofOfStake() : VerifySignature failed on coinstake %s\n", tx->GetHash().ToString());
            return false;
        }
        // Get transaction index for the previous transaction
        
        auto itr = mapBlockIndex.find(hash);
        if(itr == mapBlockIndex.end())
        {
            LogPrintf("CheckProofOfStake() : blockindex not found hash = %s\n", hash.ToString().c_str());
            return false;
        }
            
        CBlockIndex* pindex = (*itr).second;
        CBlock block;
        if (!ReadBlockFromDisk(block, pindex, Params().GetConsensus()))
        {
            LogPrintf("CheckProofOfStake() : block not found db hash = %s\n", hash.ToString().c_str());
            return true;
        }

        nTimeBlockFrom = block.GetBlockTime();
        nTxPrevOffset = GetSizeOfCompactSize(block.vtx.size()) + sizeof(CBlockHeader);
        nAmount = txTmp->vout[txin.prevout.n].nValue;
        n = txin.prevout.n;
        m_txdata[hash] = tuple<uint32_t, unsigned int, CAmount, uint64_t>(nTimeBlockFrom, nTxPrevOffset, nAmount, n);
    }

    if (!CheckStakeKernelHash(nBits, nTimeBlockFrom, nTxPrevOffset, nAmount, n, nBlockTime, hashProofOfStake))
        return false;
        //return state.DoS(1, error("CheckProofOfStake() : INFO: check kernel failed on coinstake %s, hashProof=%s", tx->GetHash().ToString(), hashProofOfStake.ToString())); // may occur during initial download or if behind on block chain sync

    return true;
    
}
