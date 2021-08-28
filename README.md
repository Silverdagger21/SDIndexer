# SDIndexer

SDIndexer is an application that creates an index, based on file contents.
You can query the index and it will provide you with the files most relevant to your query. 
You can use either the GUI the console or command line arguments. 


## Usage
To start with:

* Select a **directory** to index or file to load the index
* Type your **query** and the app will give you the most **relevant** documents



 There are a few **options** to choose from as well:

 * convert all words to lowercase
 * exclude numbers
 * exclude symbols

 You can also **save** the index to a file and then later **load** it.


## Implemenetation

The `console` implementation depends mainly on standard libraries with the exception of the `conio.h` library used in windows to make the console more responsive.

The `GUI` is implemented using the [wxWidgets](https://github.com/wxWidgets/wxWidgets)
framework, which is cross platform.


### Final notes


