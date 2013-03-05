# -*- fill-column: 100; -*-
#
# Simple wrapper for ISAAC PRNG. This is suitable for cryptographic use.
#
# dmb - June 2012 - Copyright (C) 2012 Arcode Corporation
#
from _isaac import _isaac


class isaac(_isaac):
    def __init__(self, seed=None):
        """
        Create a new ISAAC pseudo-random number generator. Pass an optional string of arbitrary
        length to seed the generator; if you do not, the generator will be seeded with a value
        derived either the system's cryptographic random number generator (if available), or
        the current time.
        """
        if seed is None:
            try:
                from os import urandom
                seed = urandom(1024)
            except:
                print "WARNING: using current time to seed ISAAC random number generator"
                import time
                import hashlib
                hasher = hashlib.sha512()
                now = time.time()
                hasher.update(str(now))
                now = time.time()
                hasher.update(str(now))
                seed = hasher.digest()

        # Seed the generator with the string
        _isaac.__init__(self, seed)

    def getrandbits(self, bits):
        "Return a value consisting of the specified number of pseudo-random bits."
        v = 0
        while bits >= 32:
            v <<= 32
            v |= self.rand()
            bits -= 32
        if bits >= 16:
            v <<= 16
            v |= (self.rand() & 0xFFFF)
            bits -= 16
        if bits >= 8:
            v <<= 8
            v |= (self.rand() & 0xFF)
            bits -= 8
        if bits >= 4:
            v <<= 4
            v |= (self.rand() & 0xF)
            bits -= 4
        if bits >= 2:
            v <<= 2
            v |= (self.rand() & 0x3)
            bits -= 2
        if bits >= 1:
            v <<= 1
            v |= (self.rand() & 1)
            bits -= 1
        return v

    def random_string(self, N, allowable_characters=None):
        "Return a pseudo-random string containing the specified number of bytes."
        if allowable_characters is None:
            return super(isaac, self).random_string(N)

        l = len(allowable_characters)
        return "".join([allowable_characters[self.rand() % l] for i in range(N)])
