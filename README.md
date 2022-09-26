First build and install the arrow json lib. I choose version 8.0.0.
Install it at ~/software/arrow-8.0.0/

```bash
mkdir -p ~/workspace
cd ~/workspace
git clone --single-branch -b release-8.0.0 https://github.com/apache/arrow.git
cd arrow/cpp
cmake -DARROW_JSON=ON -DARROW_OPTIONAL_INSTALL=ON -DARROW_JEMALLOC=OFF -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX=~/software/arrow-8.0.0/ .
make -j8 all install
```

Then in this project folder, build and run the program.

```bash
cd ~/workspace/arrow-helloworld
cmake .
make
./json_decimal_example test_quoted.json
```

The outpus:

```
* Reading JSON file 'test_quoted.json' into table
* Read table:
id: int32
str: string
price: decimal128(9, 2)
----
id:
  [
    [
      1,
      2
    ]
  ]
str:
  [
    [
      "Some",
      "data"
    ]
  ]
price:
  [
    [
      30.04,
      12.34
    ]
  ]
```

Test another json file with unquoted decimal values:

```
$ ./json_decimal_example test_unquoted.json 
Reading test_unquoted.json* Reading JSON file 'test_unquoted.json' into table
Invalid: JSON parse error: Column(/price) changed from string to number in row 0
```
