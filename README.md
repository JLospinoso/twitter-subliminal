# Register a Twitter Application
You'll have to go to [https://apps.twitter.com/][https://apps.twitter.com/] and register for a new app. Once you've done this,

Save your Consumer Key and Consumer Secret in the "Keys and Access Tokens" Tab.

Click "Create my access token", and save the Access Token and Access Token Secret.

After this process, you should have four (4) total tokens.

# Easiest: Run from Docker

```sh
docker run -it --rm \
    -e consumer.key="abcd123abcd123abcd123abcd123" \
    -e secret.key="aabcd123abcd123abcd123abcd123" \
    -e access.token="abcd123abcd123abcd123abcd123" \
    -e access.token.secret="abcd123abcd123abcd123" \
     quay.io/jlospinoso/twitter-subliminal:0.2.0
```

From this interactive session, you'll be able to run all of the twitter-subliminal binaries:

* tse: message encryption
* tsd: message decryption
* tsp: performance testing
* tst: unit testing
* tsl: check rate limit status with Twitter Application
* tsr: reset retweets

# Configuring twitter-subliminal
You must have a `twitter-subliminal.properties` file in the same directory as your executables. Using the
tokens that you've collected from the previous section, you can generate your own `.properties` easily
by inserting the tokens into `twitter-subliminal.properties.example` from the repo:

```properties
# Encoding defaults
blocksize : 8

# Twitter account information
consumer.key : abcd123abcd123abcd123abcd123
secret.key : abcd123abcd123abcd123abcd123
access.token : abcd123abcd123abcd123abcd123
access.token.secret : abcd123abcd123abcd123
ca.path :
lang : en

# Logging defaults
log.file : twitter-subliminal.log
log.level : information
log.pattern : %Y-%m-%d %H:%M:%S %s %p [%P %I] %t

# Performance Testing defaults
update.interval : 20
sample.time : 5
blocks.trial : 10
```

If you have a directory with Certificate Authorities available in your environment, e.g. `/etc/ssl/certs` on
*nix, you can fill in `ca.path` with this value.

`lang` can be changed to the language you'd like your original tweets to be in. See [https://dev.twitter.com/streaming/overview][https://dev.twitter.com/streaming/overview].

# Binaries
You can either use the binaries from the /bin folder, or you can build your own. Just make sure that `twitter-subliminal.properties` is in the working directory.

# Setting up your build environment
Clone the repository:

```sh
git clone git@github.com:JLospinoso/twitter-subliminal.git
```

Navigate into `twitter-subliminal`, then clone googletest:

```sh
cd twitter-subliminal
git clone git@github.com:google/googletest
```

Next, ensure [Poco][http://pocoproject.org/] is installed in your environment. You won't need
any of the Data or persistence bindings--NetSSL, Crypto, and the Core should do it. On Windows/Cygwin,
you can install this with the Cygwin installer/package manager; on OS X, you can use [homebrew][http://brew.sh/],
and on Linux you can check your favorite package manager, but it's easy enough to [install from source][http://pocoproject.org/download/].

Input your Poco configuration information into the cmake finder:

```sh
vim cmake/Modules/FindPoco.cmake
```

Edit the path to your Poco root, e.g.

```cmake
set(Poco_ROOT C:\\cygwin64)
```

or

```cmake
set(Poco_ROOT /usr/local/)
```

# Building on OS X/*nix
From `twitter-subliminal`,

```sh
mkdir build
cd build
cmake ..
make
```

This will compile and link all of your binaries into the build folder.

# Building on Windows (cygwin)
From a [Cygwin terminal][https://cygwin.com/],

```sh
mkdir build
cd build
cmake -G"Unix Makefiles" ..
make
```

This will compile and link all of your binaries into the build folder.
