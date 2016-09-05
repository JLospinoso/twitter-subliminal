# Poco_FOUND        - True when the Poco include directory is found.
# Poco_INCLUDE_DIRS - Path to where the poco include files are.
# Poco_LIBRARIES    - List of Poco libraries

set(Poco_ROOT /usr/local)
set(Poco_HEADER_SEARCH PATHS ${Poco_ROOT}/include NO_SYSTEM_PATH)
set(Poco_LIBRARY_SEARCH PATHS ${Poco_ROOT}/lib NO_SYSTEM_PATH)

message(STATUS "Poco Library Search Path: ${Poco_LIBRARY_SEARCH}")
find_path(Poco_INCLUDE_DIR Poco/Poco.h DOC "Path to Poco headers" ${Poco_HEADER_SEARCH})

find_library(Poco_FOUNDATION_LIBRARY
        NAMES PocoFoundation PocoFoundationd
        DOC "Path to Poco Foundation library"
        ${Poco_LIBRARY_SEARCH}
        )
if(Poco_FOUNDATION_LIBRARY)
    set(Poco_LIBRARIES ${Poco_FOUNDATION_LIBRARY})
else()
    message("[-] Could not find PocoFoundation")
endif()

find_library(Poco_UTIL_LIBRARY
        NAMES PocoUtil PocoUtild
        DOC "Path to Poco Utility library"
        ${Poco_LIBRARY_SEARCH}
        )
if(Poco_UTIL_LIBRARY)
    set(Poco_LIBRARIES ${Poco_LIBRARIES} ${Poco_UTIL_LIBRARY})
else()
    message("[-] Could not find PocoUtil")
endif()

find_library(Poco_XML_LIBRARY
        NAMES PocoXML PocoXMLd
        DOC "Path to Poco XML library"
        ${Poco_LIBRARY_SEARCH}
        )
if(Poco_XML_LIBRARY)
    set(Poco_LIBRARIES ${Poco_LIBRARIES} ${Poco_XML_LIBRARY})
else()
    message("[-] Could not find PocoXML")
endif()

find_library(Poco_JSON_LIBRARY
        NAMES PocoJSON PocoJSONd
        DOC "Path to Poco JSON library"
        ${Poco_LIBRARY_SEARCH}
        )
if(Poco_JSON_LIBRARY)
    set(Poco_LIBRARIES ${Poco_LIBRARIES} ${Poco_JSON_LIBRARY})
else()
    message("[-] Could not find PocoJSON")
endif()

find_library(Poco_CRYPTO_LIBRARY
        NAMES PocoCrypto PocoCryptod
        DOC "Path to Poco Crypto library"
        ${Poco_LIBRARY_SEARCH}
        )
if(Poco_CRYPTO_LIBRARY)
    set(Poco_LIBRARIES ${Poco_LIBRARIES} ${Poco_CRYPTO_LIBRARY})
else()
    message("[-] Could not find PocoCrypto")
endif()

find_library(Poco_DATA_LIBRARY
        NAMES PocoData PocoDATAd
        DOC "Path to Poco Data library"
        ${Poco_LIBRARY_SEARCH}
        )
if(Poco_DATA_LIBRARY)
    set(Poco_LIBRARIES ${Poco_LIBRARIES} ${Poco_DATA_LIBRARY})
else()
    message("[-] Could not find PocoData")
endif()

#find_library(Poco_DATAMYSQL_LIBRARY
#        NAMES PocoDataMySQL PocoDataMySQLd
#        DOC "Path to Poco DATAMYSQL library"
#        ${Poco_LIBRARY_SEARCH}
#        )
#if(Poco_DATAMYSQL_LIBRARY)
#    set(Poco_LIBRARIES ${Poco_LIBRARIES} ${Poco_DATAMYSQL_LIBRARY})
#else()
#    message("[-] Could not find PocoDataMySQL")
#endif()

#find_library(Poco_DATAODBC_LIBRARY
#        NAMES PocoDataODBC PocoDataODBCd
#        DOC "Path to Poco DataODBC library"
#        ${Poco_LIBRARY_SEARCH}
#        )
#if(Poco_DATAODBC_LIBRARY)
#    set(Poco_LIBRARIES ${Poco_LIBRARIES} ${Poco_DATAODBC_LIBRARY})
#else()
#    message("[-] Could not find PocoDataODBC")
#endif()

#find_library(Poco_DATASQLLITE_LIBRARY
#        NAMES PocoDataSQLLite PocoDataSQLLited
#        DOC "Path to Poco DataSQLLite library"
#        ${Poco_LIBRARY_SEARCH}
#        )
#if(Poco_DATASQLLITE_LIBRARY)
#    set(Poco_LIBRARIES ${Poco_LIBRARIES} ${Poco_DATASQLLITE_LIBRARY})
#else()
#    message("[-] Could not find PocoDataSQLLite")
#endif()

#find_library(Poco_MONGODB_LIBRARY
#        NAMES PocoMongoDB PocoMongoDBd
#        DOC "Path to Poco MONGODB library"
#        ${Poco_LIBRARY_SEARCH}
#        )
#if(Poco_MONGODB_LIBRARY)
#    set(Poco_LIBRARIES ${Poco_LIBRARIES} ${Poco_MONGODB_LIBRARY})
#else()
#    message("[-] Could not find PocoMongoDB")
#endif()

find_library(Poco_NET_LIBRARY
        NAMES PocoNet PocoNetd
        DOC "Path to Poco Net library"
        ${Poco_LIBRARY_SEARCH}
        )
if(Poco_NET_LIBRARY)
    set(Poco_LIBRARIES ${Poco_LIBRARIES} ${Poco_NET_LIBRARY})
else()
    message("[-] Could not find PocoNet")
endif()

find_library(Poco_NETSSL_LIBRARY
        NAMES PocoNetSSL PocoNetSSLd
        DOC "Path to Poco NetSSL library"
        ${Poco_LIBRARY_SEARCH}
        )
if(Poco_NETSSL_LIBRARY)
    set(Poco_LIBRARIES ${Poco_LIBRARIES} ${Poco_NETSSL_LIBRARY})
else()
    message("[-] Could not find PocoNetSSL")
endif()

find_library(Poco_NETSSL_LIBRARY
        NAMES PocoNetSSL PocoNetSSLd
        DOC "Path to Poco NetSSL library"
        ${Poco_LIBRARY_SEARCH}
        )
if(Poco_NETSSL_LIBRARY)
    set(Poco_LIBRARIES ${Poco_LIBRARIES} ${Poco_NETSSL_LIBRARY})
endif()

find_library(Poco_ZIP_LIBRARY
        NAMES PocoZip PocoZipd
        DOC "Path to Poco Zip library"
        ${Poco_LIBRARY_SEARCH}
        )
if(Poco_ZIP_LIBRARY)
    set(Poco_LIBRARIES ${Poco_LIBRARIES} ${Poco_ZIP_LIBRARY})
else()
    message("[-] Could not find PocoZip")
endif()
