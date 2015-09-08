# Sailfish 
##### Also known as Sailfish C ,It's a Network C library, the implementation based on Berkeley Socket API.
#
##### Note
#    
#
    This project is still in development.. Please don't expect to be bug free and also feel free to test it on your own risk.
![SailFish C](http://www.xtremesignandgraphic.com/assets/images/sailfish.jpg)
#
#

####The library will support:

|            Protocol           	| IPV4(AF_INET) 	| IPV6(AF_INET6) 	| UNIX(AF_LOCAL) 	| ROUTE (AF_ROUTE) 	| KEY(AF_KEY) 	|
|:-----------------------------:	|---------------	|----------------	|----------------	|------------------	|-------------	|
| TCP(SOCK_STREAM)              	| x             	| x              	| x              	| -                	| -            	|
| UDP(SOCK_DGRAM)               	| x             	| x              	| x              	| -                	| -            	|
| SCTP(one-one SOCK_STREAM)     	| x             	| ?              	| ?              	| -                	| -            	|
| SCTP(one-many SOCK_SEQPACKET) 	| x             	| ?              	| ?              	| -                	| -            	|
| Raw(SOCK_RAW)                 	| x             	| x              	| -              	| ?                 	| ?            	|



#### MAP:
    
    x  ->  yes
    ?  ->  maybe in the future
    -  ->  (No!)



