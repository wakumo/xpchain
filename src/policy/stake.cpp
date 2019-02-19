// Copyright (c) 2018 The XPChain Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <policy/stake.h>

#include <crypto/sha256.h>
#include <hash.h>
#include <primitives/block.h>
#include <primitives/transaction.h>
#include <pubkey.h>
#include <script/interpreter.h>
#include <script/standard.h>
#include <stdio.h>
#include <key_io.h>

bool IsDestinationSame(const CScript& a, const CScript& b)
{
    txnouttype aType, bType;
    std::vector <std::vector<unsigned char>> aSol, bSol;

    if (!Solver(a, aType, aSol) || !Solver(b, bType, bSol)) {
        return false;
    }

    if (aType != bType) {
        return false;
    }

    if (aSol.size() != 1 || bSol.size() != 1) {
        return false;
    }

    if (aSol[0] != bSol[0]) {
        return false;
    }

    return true;
}