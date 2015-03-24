# SBB file format #

SBB  (Stream of Binary Blocks)

A SBB file has one header and blocks of data to be loaded

## Header Format ##

|position|field name| type|comment|
|:-------|:---------|:----|:------|
|0..2 |sbb\_version| char(3)|always "SB" + version number (2.1=0x21)|
|3..6 |machine| char(4)|ZX , CPC , MSX, 81|
|7 |model| char |model 48k, +2A. 464 ...|
|8..14 |extre\_info| char(7)|Ready  For Future Uses|
|15 |ei\_di| char|Enable disable interrupts between blocks|
|16..31 | name| char(16)|name|
|32 | locate| char|Most Significative Byte for the address of loading rutine|
|33 | origin| char| sbb was built from tap, bin , sna ...|
|34 | n\_blocks| char|number of blocks |
|35 | poke\_ffff| char|msx pagination (if 0, Poke -1 is not modified)|
|36..37| clear | ushort | clear or SP (if 0, SP is not modified) |
|38..39| n\_usr| ushort | execution address after loading all bloacks(if 0, it is not an executable ) |


## Block format ##

One of this for each block

|position|field name| type|comment|
|:-------|:---------|:----|:------|
|0..15|blockname| char (16) | description|
|16..17|size| ushort |number of bytes for data |
|18..19|param3| ushort | aux parammter |
|20|block\_type| char| type|
|21|h\_chksum| char| checksum for header of block =((ini-1) XOR jump XOR exec)  |
|22..23|ini| ushort | start memory address for data |
|24|jump| char|  0xC3 or 0xCD for an executable block , 0x01 or oxc9 in other case|
|25..26|exec| ushort | execution address for block. If zero, it is not an executable block|
|27|d\_chksum| char | checksum for data |
|28..28+size-1|data | char (size) | memory block |




.