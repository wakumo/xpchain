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

bool CheckStakeKernelHash(unsigned int nBits, uint32_t nTimeBlockFrom, unsigned int nTxPrevOffset, CAmount nAmount, uint64_t n, uint32_t nTimeTx, uint256& hashProofOfStake)
{
    if (nTimeBlockFrom + Params().GetConsensus().nStakeMinAge > nTimeTx) // Min age requirement
        return false;

    arith_uint256 bnTargetPerCoinDay;
    bnTargetPerCoinDay.SetCompact(nBits);

    int64_t nTimeWeight = min((int64_t)nTimeTx - nTimeBlockFrom, Params().GetConsensus().nStakeMaxAge) - Params().GetConsensus().nStakeMinAge;

    arith_uint256 bnCoinDayWeight = arith_uint256(nAmount) * nTimeWeight / COIN / (24 * 60 * 60);

    // Calculate hash
    CDataStream ss(SER_GETHASH, 0);

    ss << nBits << nTimeBlockFrom << nTxPrevOffset << nTimeBlockFrom << n << nTimeTx;

    hashProofOfStake = Hash(ss.begin(), ss.end());

    if (arith_uint512(UintToArith256(hashProofOfStake))> arith_uint512(bnCoinDayWeight) * arith_uint512(bnTargetPerCoinDay))
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

        if(txTmp->GetHash() != txin.prevout.hash)
        {
            return false;
        }

        if(hash == uint256())
        {
            return false;
        }

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
        if(pindex->GetBlockHash() != hash)
        {
            return false;
        }

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

    return true;

}
