# Co_requests

NOTE: This project is currently an exploration phase. Do not use it for anything. Interfaces will change. 

This project serves two goals:
1. Provide a high level async requests library with an interface similar to the Python requests module.
2. Learn coroutines (and anything other features from C++20 while I'm at it.)

## To do
1. Design API
1. Decide on backend for networking. 
    1. Create a layer to easily switch out backend. 
    1. Options: httplib, libcurl, asio, etc.
1. Stand up tests infrastructure
