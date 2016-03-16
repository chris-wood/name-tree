#include <string>

#include <cryptopp/base64.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <cryptopp/pwdbased.h>
#include <cryptopp/sha.h>

#include "hasher.hpp"

using namespace std;

Hasher::Hasher() {
    // pass
}

string
Hasher::Hash(string aString) {

    cout << "HASHING => " << aString << endl;

    string digest;
    CryptoPP::SHA256 hash;

    CryptoPP::StringSource foo(aString, true,
    new CryptoPP::HashFilter(hash,
      new CryptoPP::Base64Encoder (
         new CryptoPP::StringSink(digest))));

    return digest;
}
