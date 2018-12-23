XPChain Core integration/staging tree
=====================================

[![Build Status](https://travis-ci.org/xpc-wg/xpchain.svg?branch=master)](https://travis-ci.org/xpc-wg/xpchain)

https://www.xpchain.io/

What is XPChain?
----------------

XPChain, which is usually abbreviated XPC, is a cryptocurrency based on Bitcoin 0.17.0.

XPChain introduces new proof-of-stake consensus as a secutiry model, which solves
“nothing at stake” issue in a way proposed by Peercoin community, and enables you
to distribute your staking rewards to anyone in the world.

XPChain Core is the name of open source
software which enables the use of this currency.

For more information, <!-- as well as an immediately useable, binary version of
the XPChain Core software, see https://bitcoincore.org/en/download/, --> read the [original whitepaper](https://www.xpchain.io/?loc=lnkwhitepaper).

License
-------

XPChain Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

Development Process
-------------------

The `master` branch is regularly built and tested, but is not guaranteed to be
completely stable. [Tags](https://github.com/xpc-wg/xpchain/tags) are created
regularly to indicate new official, stable release versions of XPChain Core.

The contribution workflow is described in [CONTRIBUTING.md](CONTRIBUTING.md).

Testing
-------

Testing and code review is the bottleneck for development; we get more pull
requests than we can review and test on short notice. Please be patient and help out by testing
other people's pull requests, and remember this is a security-critical project where any mistake might cost people
lots of money.

### Automated Testing

Developers are strongly encouraged to write [unit tests](src/test/README.md) for new code, and to
submit new unit tests for old code. Unit tests can be compiled and run
(assuming they weren't disabled in configure) with: `make check`. Further details on running
and extending unit tests can be found in [/src/test/README.md](/src/test/README.md).

There are also [regression and integration tests](/test), written
in Python, that are run automatically on the build server.
These tests can be run (if the [test dependencies](/test) are installed) with: `test/functional/test_runner.py`

The Travis CI system makes sure that every pull request is built for Windows, Linux, and macOS, and that unit/sanity tests are run automatically.

### Manual Quality Assurance (QA) Testing

Changes should be tested by somebody other than the developer who wrote the
code. This is especially important for large or high-risk changes. It is useful
to add a test plan to the pull request description if testing the changes is
not straightforward.

Translations
------------

While Bitcoin has [their Transifex page](https://www.transifex.com/projects/p/bitcoin/),
XPChain doesn't have such pages of external translation web service (e.g. Transifex or Crowdin). If you want to make some change as well as add new translations, just create a new GitHub pull request
to whose title is prefixed “Translation:”.
Read [translation_process.md](https://github.com/xpc-wg/xpchain/blob/master/doc/translation_process.md) for details.
