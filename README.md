# SDIndexer

SDIndexer is an application that creates an index, based on file contents.
You can query the index and get the files most relevant to your query. 
You can use the **GUI** or **console**, and compile it for windows or linux. 


## Usage
To start with:

* Select a **directory** to index or file to load the index
* Type your **query** and the app will give you the most **relevant** documents

 There are a few **options** to choose from:

 * convert all text to lowercase
 * exclude numbers
 * exclude symbols
 * index subdirectories
 * include / exclude certain filetypes from indexing

 You can also **save** the index to a file and **load** it later.


## Implemenetation

The `console` implementation depends mainly on standard libraries with the exception of the `conio.h` library 
which is only used when when compiled for windows to make the console more responsive.

The `GUI` implementation uses the [wxWidgets](https://github.com/wxWidgets/wxWidgets)
framework, which should work for most platforms.

