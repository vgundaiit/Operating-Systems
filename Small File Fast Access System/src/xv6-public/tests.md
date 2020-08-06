
# Tests for small files and directories in Xv6

* **Name : Nithin Rao**
* **CWID : A20446755**

<br/>

* **Name : Varun Gunda**
* **CWID : A20453991**

<br/>

NOTE : The answer to ```Why a test is written?``` is answered under ```PURPOSE``` of output for each test. 

## ```1. Create small directory```
 
```
    -> A. Creating a small directory in a normal directory
    -> B. Creating a small directory in a small directory
    -> C. Creating a normal directory in a small directory
```

### Test 1.A : 


    $ mkSFdir sdir
    $ cd sdir
    $ ls
    .              4 23 32
    ..             1 1 512
    $ cd ..
    $ ls
    .              1 1 512
    ..             1 1 512
    README         2 2 2170
    cat            2 3 13644
    echo           2 4 12652
    forktest       2 5 8088
    grep           2 6 15520
    init           2 7 13236
    kill           2 8 12708
    ln             2 9 12604
    ls             2 10 14788
    mkdir          2 11 12788
    mkSFdir        2 12 12800
    testlink       2 13 13104
    rm             2 14 12764
    sh             2 15 23252
    stressfs       2 16 13436
    usertests      2 17 56364
    wc             2 18 14184
    zombie         2 19 12428
    touch          2 20 12788
    writegarb      2 21 13244
    console        3 22 0
    sdir           4 23 32

***Purpose*** : Creates a small directory under the root directory to check that a small directory can be created in a normal directory.***We cam see sdir directory is of TYPE 4, which is small directory.***

### Test 1.B : 


    $ mkSFdir sdir
    $ cd sdir
    $ mkSFdir sdir2
    $ cd sdir
    $ ls
    .              4 24 32
    ..             4 23 48

***Purpose*** : Creates a small directory under a small directory to check that a small directory can be created inside a small directory in a hierarchy.

### Test 1.C : 

    $ mkSFdir sdir
    $ cd sdir
    $ mkdir ndir
    $ cd ndir
    $ ls
    .              1 25 32
    ..             4 23 48

***Purpose*** : Creates a normal directory under a small directory to check that a normal directory can be created in a small directory. **As there was no constraint that normal dirs cannot exist under small dirs**.
    

## ```2. Delete small directory```
 
```
    -> A. Deleting a small directory
```

### Test 2.A : 

    $ mkSFdir sdir
    $ ls
    .              1 1 512
    ..             1 1 512
    README         2 2 2170
    cat            2 3 13644
    echo           2 4 12652
    forktest       2 5 8088
    grep           2 6 15520
    init           2 7 13236
    kill           2 8 12708
    ln             2 9 12604
    ls             2 10 14788
    mkdir          2 11 12788
    mkSFdir        2 12 12800
    testlink       2 13 13104
    rm             2 14 12764
    sh             2 15 23252
    stressfs       2 16 13436
    usertests      2 17 56364
    wc             2 18 14184
    zombie         2 19 12428
    touch          2 20 12788
    writegarb      2 21 13244
    console        3 22 0
    sdir           4 23 64
    $ cd sdir
    $ ls
    .              4 23 32
    ..             1 1 512
    $ cd ..
    $ rm sdir
    $ ls
    .              1 1 512
    ..             1 1 512
    README         2 2 2170
    cat            2 3 13644
    echo           2 4 12652
    forktest       2 5 8088
    grep           2 6 15520
    init           2 7 13236
    kill           2 8 12708
    ln             2 9 12604
    ls             2 10 14788
    mkdir          2 11 12788
    mkSFdir        2 12 12800
    testlink       2 13 13104
    rm             2 14 12764
    sh             2 15 23252
    stressfs       2 16 13436
    usertests      2 17 56364
    wc             2 18 14184
    zombie         2 19 12428
    touch          2 20 12788
    writegarb      2 21 13244
    console        3 22 0

***Purpose*** : Deletes a small directory under a normal directory to check that a small directory can be deleted using ***rm*** command. 
    

## ```3. Create```
 
```
    -> A. Creating a file inside a small directory 
    -> B. Creating a file outside a small directory
```

### Test 3.A : 

    $ mkSFdir sdir
    $ cd sdir
    $ touch stestfile
    $ ls
    .              4 23 32
    ..             1 1 512
    stestfile      5 24 0

***Purpose*** : To check if creating a file under a small directory creates a small file.***The above output shows that a small file of TYPE 5 is created inside sdir directory.***
    

### Test 3.B : 

    $ mkdir ndir
    $ cd ndir
    $ touch ntestfile
    $ ls
    .              4 25 32
    ..             1 1 512
    ntestfile      2 26 0

***Purpose*** : To check if creating a file under a normal directory creates a normal file.***The above output shows that a normal file of TYPE 2 is created inside ndir directory.***    


## ```4. Open```
 
```
    -> A. Opening a small file in a small directory
    -> B. Opening a normal file in a small directory.
```

### Test 4.A : 

    $ mkSFdir sdir
    $ cd sdir
    $ touch stestfile
    $ writegarb stestfile
    Write size : 51
    $ cat stestfile
    Machines take me by surprise with great frequency.

***Purpose*** : To check if opening a small file under a small directory works.***The above output shows that a write was executed on a small file of TYPE 5 created under sdir small directory.***  
```
The above test also tests for :
    5.A : Reading less than 52 bytes from small files,
    6.A :  Writing less than 52 bytes from small files
```

### Test 4.B : 

    $ mkSFdir sdir
    $ cd sdir
    $ touch stestfile
    $ ls
    .              4 31 48
    ..             1 1 512
    stestfile      5 32 0
    $ writegarb stestfile more_than_52
    Write size : 59
    $ ls
    .              4 31 48
    ..             1 1 512
    stestfile      2 32 59
    $ cat stestfile
    Machines take me by surprise with great frequency testing.


***Purpose*** : To check if opening a noraml file under a small directory works.***The above output shows that a write was executed on a small file of TYPE 5 created under sdir small directory, BUT once we write more than 52 bytes, it gets converted into a normal file of TYPE 2!.***  
```
The above test also tests for :
    6.C :  Writing less than 52 bytes from small files
```

## ```5. Read```
 
```
    -> A. Reading less than 52 bytes from small files 
    -> B. Reading exactly 52 bytes from small files
    -> C. Reading more than 52 bytes from small files
```

### Test 5.A :

    Tested under 4.A

***Purpose*** : To check if reading less than 52 bytes from a small file works.


### Test 5.B :

    $ mkSFdir sdir
    $ cd sdir
    $ touch stestfile
    $ writegarb stestfile exactly_52
    $ readtest stestfile 52
    Read 52 bytes.

***Purpose*** : To check if reading exactly 52 bytes from a small file works and soesn't run into corner case issues.

### Test 5.C :

    $ mkSFdir sdir
    $ cd sdir
    $ touch stestfile
    $ writegarb stestfile exactly_52
    $ readtest stestfile 60
    Read 51 bytes.

***Purpose*** : To check if reading more than 52 bytes from a small file gives an error or just reads 52 bytes or less (how much ever exists!).

## ```6. Write```
 
```
    -> A. Writing less than 52 bytes from small files 
    -> B. Writing exactly 52 bytes from small files
    -> C. Writing more than 52 bytes from small files
```

### Test 6.A :

    Tested under 4.A

***Purpose*** : To check if writing less than 52 bytes to a small file works.  

### Test 6.B :

    $ mkSFdir sdir
    $ cd sdir
    $ touch stestfile
    $ writegarb stestfile exactly_52
    $ cat stestfile
    Machines take me by surprise with great frequency .
    $ ls
    .              4 24 48
    ..             1 1 512
    tf             5 25 52

***Purpose*** : To check if writing exactly 52 bytes to a small file works runs into any corner case issues.  

### Test 6.C :

    Tested under 4.B
    todo
    
***Purpose*** : ***To check if writing more than 52 bytes to a small file gives an error or converts it into a normal file from small file, we also read from it to make sure the contents are transferred to the disk's data blocks.***  


## ```7. Delete```
 
```
    -> A. Deleting a small file 
```
### Test 7.A :

    $ mkSFdir sdir
    $ cd sdir
    $ touch stestfile
    $ ls
    .              4 31 48
    ..             1 1 512
    stestfile      5 32 0
    $ rm stestfile
    $ ls
    .              4 31 48
    ..             1 1 512

***Purpose*** : To check if deleting a small file works using the ***rm*** command which ultimately uses the unlink system call to delete a file.```

<br/><br/>

## ***All the above mentioned commands works in our Xv6 copy, so please don't hesistate to try them out!***


## Thank you!