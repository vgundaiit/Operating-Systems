# Programming Assignment 4 CS450 OS

* **Name : Nithin Rao**
* **CWID : A20446755**


* **Name : Varun Gunda**
* **CWID : A20453991**


<br/>

## Directory Organization
For answers to written questions, there are 2 different formats for the same answer, an HTML file and a PDF file, use whichever is convinient.

* **src/** - 
    * **xv6-public/** - Complete source code of Xv6 with the assignment changes.
* **doc** - Contains the documents containing answers to the programming assignment
    * **design/** 
        * *design.html* - HTML file with the walkthrough of files changed to implement systemcalls and some explanantion of the design decisions.
        * *design.pdf* - PDF file with the walkthrough of files changed to implement systemcalls and some explanantion of the design decisions.
    * **tests/**
        * *tests.html* - HTML file describing the test cases and its output.
        * *tests.pdf* - PDF file describing the test cases and its output.

### Steps to setup working build with code provided in src/xv6-public/ : 

*NOTE : I have copied the entire source code of xv6 under the xv6-public folder.*
1) **cd into the xv6-public directory from assignment root dir :**
    ```shell
    cd src/xv6-public/
    ```

1) **Running the following three commands will build Xv6 and bring up a shell for us to try our test program :**

    ```shell
    $ make clean
    $ make qemu
    ```

1) **Once we have Xv6 running, we go ahead and run the test programs available to us:**

    1) **mkSFdir <DIR_NAME>** Create a small file directory.
    1) **touch <FILE_NAME>** Create a file inside a dir (automatically create small file in a small dir and a normal file in a normal dir).
    1) **writegarb <FILE_NAME> <OPTIONAL_BYTES>** Writes a string of 51 bytes by default into the file specified, OPTIONAL_BYTES are "exactly_52" and "more_than_52" to specify the variation in bytes written.
    1) **readtest <FILE_NAME> <NO_BYTES_TO_READ>** Reads NO_BYTES_TO_READ from the file specified and prints the no of bytes it could actually read.
    5) **testlink** Tests the linking of files across different directory types.

    ## Examples for the above commands: 
    ```shell
    $ mkSFdir sdir
    $ cd sdir
    $ touch small_file
    $ writegarb small_file exactly_52
    Write size : 52
    $ cat small_file
    Machines take me by surprise with great frequency.
    $ readtest small_file 60
    Read 52 bytes.
    $ cd ..
    $ testlink 
    linking done
    ```

### Please take a look at tests.html/tests.pdf document to get a better picture of what the above commands do.

## Thank you!