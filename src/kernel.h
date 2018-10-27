#ifndef BITCOIN_KERNEL_H
#define BITCOIN_KERNEL_H
#include<primitives/block.h>
#include <chain.h>

class CValidationState;
class uint256;

bool CheckStakeKernelHash(unsigned int nBits, const CBlock& blockFrom, unsigned int nTxPrevOffset, const CTxOut& txOutPrev, const COutPoint& prevout, uint32_t nTimeTx, uint256& hashProofOfStake);
bool CheckProofOfStake(CValidationState& state, const CTransactionRef& tx, unsigned int nBits, uint256& hashProofOfStake, unsigned int nBlockTime);
#endif // BITCOIN_KERNEL_H