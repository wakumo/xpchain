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

bool AddressesEqual(const CScript& a, const CScript& b)
{
    txnouttype aType, bType;
    std::vector<std::vector<unsigned char>> aSolutions, bSolutions;

    if(!Solver(a, aType, aSolutions) || !Solver(b, bType, bSolutions))
    {
        return false;
    }

    if(aSolutions.size() != 1 || bSolutions.size() != 1)
    {
        return false;
    }

    //addresses of a,b are the same
    //TODO: fix
    CTxDestination aAddress, bAddress;
    if(!ExtractDestination(a, aAddress) || !ExtractDestination(b, bAddress))
    {
        return false;
    }

    //printf("a = %s b = %s\n", EncodeDestination(aAddress).c_str(), EncodeDestination(bAddress).c_str());
    return aAddress == bAddress;
}
