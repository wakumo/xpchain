class CValidationState;
class uint256;

const uint64_t nStakeMinAge = 0;
const uint64_t nStakeMaxAge = 60 * 60 * 24 * 100;
bool CheckStakeKernelHash();
bool CheckProofOfStake(CValidationState& state, uint256 txhash);