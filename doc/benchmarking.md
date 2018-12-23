Benchmarking
============

XPChain Core has an internal benchmarking framework, with benchmarks
for cryptographic algorithms (e.g. SHA1, SHA256, SHA512, RIPEMD160), as well as the rolling bloom filter.

Running
---------------------
After compiling xpchain-core, the benchmarks can be run with:

    src/bench/bench_bitcoin

The output will look similar to:
```
# Benchmark, evals, iterations, total, min, max, median
AssembleBlock, 5, 700, 2.01584, 0.000559747, 0.000631826, 0.000561777
Base58CheckEncode, 5, 320000, 4.79692, 2.9788e-006, 3.04039e-006, 2.98956e-006
Base58Decode, 5, 800000, 3.32035, 8.25575e-007, 8.329e-007, 8.31716e-007
Base58Encode, 5, 470000, 4.50478, 1.91321e-006, 1.92815e-006, 1.91496e-006
Bech32Decode, 5, 800000, 1.75859, 4.36775e-007, 4.41405e-007, 4.40204e-007
Bech32Encode, 5, 800000, 2.71283, 6.69444e-007, 6.83854e-007, 6.81234e-007
BenchLockedPool, 5, 1300, 6.41475, 0.000979253, 0.000996209, 0.000982711
BnBExhaustion, 5, 650, 2.78485, 0.000845226, 0.000867372, 0.000855814
CCheckQueueSpeedPrevectorJob, 5, 1400, 14.0175, 0.00193954, 0.00204214, 0.00201775
CCoinsCaching, 5, 170000, 4.41171, 5.16908e-006, 5.22441e-006, 5.18051e-006
CoinSelection, 5, 650, 0.726855, 0.00022065, 0.000227017, 0.000222938
DeserializeAndCheckBlockTest, 5, 160, 4.42589, 0.00550796, 0.00557173, 0.00553254
DeserializeBlockTest, 5, 130, 3.2398, 0.00492961, 0.00506123, 0.00498081
FastRandom_1bit, 5, 440000000, 4.02023, 1.82147e-009, 1.84382e-009, 1.82338e-009
FastRandom_32bit, 5, 110000000, 4.90817, 8.88595e-009, 8.9918e-009, 8.90455e-009
MempoolEviction, 5, 41000, 3.66312, 1.76892e-005, 1.80922e-005, 1.78656e-005
MerkleRoot, 5, 800, 4.69078, 0.00116793, 0.00118763, 0.00116862
PrevectorClearNontrivial, 5, 28300, 11.8115, 8.2415e-005, 8.6479e-005, 8.30196e-005
PrevectorClearTrivial, 5, 88600, 26.5331, 5.8358e-005, 6.08788e-005, 6.00798e-005
PrevectorDeserializeNontrivial, 5, 6800, 2.54929, 7.34222e-005, 7.65813e-005, 7.44759e-005
PrevectorDeserializeTrivial, 5, 52000, 3.63727, 1.37805e-005, 1.42163e-005, 1.39283e-005
PrevectorDestructorNontrivial, 5, 28800, 12.4744, 8.48428e-005, 8.9182e-005, 8.6548e-005
PrevectorDestructorTrivial, 5, 88900, 26.2656, 5.8044e-005, 6.01615e-005, 5.89761e-005
PrevectorResizeNontrivial, 5, 28900, 3.19566, 2.18041e-005, 2.23964e-005, 2.2185e-005
PrevectorResizeTrivial, 5, 90300, 3.44157, 7.30212e-006, 8.24399e-006, 7.47901e-006
RIPEMD160, 5, 440, 5.17109, 0.00232664, 0.00237473, 0.00234482
RollingBloom, 5, 1500000, 3.69853, 4.89486e-007, 4.97493e-007, 4.90981e-007
SHA1, 5, 570, 5.00673, 0.00172318, 0.00182217, 0.00174249
SHA256, 5, 340, 5.1156, 0.00297841, 0.00307219, 0.00299352
SHA256D64_1024, 5, 7400, 5.01395, 0.000130739, 0.000139482, 0.000136088
SHA256_32b, 5, 4700000, 5.4757, 2.24812e-007, 2.47501e-007, 2.31665e-007
SHA512, 5, 330, 4.94175, 0.0029053, 0.00315566, 0.00298233
SipHash_32b, 5, 40000000, 5.17803, 2.56467e-008, 2.61455e-008, 2.59431e-008
Sleep100ms, 5, 10, 5.03017, 0.100434, 0.100752, 0.100623
Trig, 5, 12000000, 2.53451, 4.1733e-008, 4.26549e-008, 4.23175e-008
VerifyScriptBench, 5, 6300, 4.7698, 0.000150114, 0.000153317, 0.000151091
```

Help
---------------------
`-?` will print a list of options and exit:

    src/bench/bench_bitcoin -?

Notes
---------------------
More benchmarks are needed for, in no particular order:
- Script Validation
- CCoinDBView caching
- Coins database
- Memory pool
- Wallet coin selection
