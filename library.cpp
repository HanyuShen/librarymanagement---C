/* complete this file */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "library.h"

void Document::updateTitle(const char *newTitle){
	_title = new char[strlen(newTitle) + 1];
	strcpy(_title,newTitle);
}

void Document::updateYear(int newYear){
	_year = newYear;
}

void Document::updateQuantity(int newQuantity){
	_quantity = newQuantity;
}

char *Document::getTitle(){
	return _title;
}

int Document::getYear(){
	return _year;
}

int Document::getQuantity(){
	return _quantity;
}

int Document::borrowDoc(){
	if(this->_quantity > 0 ){
		this->_quantity--;
		return 0;
	}
	return 1;
}

void Document::returnDoc(){
	this->_quantity++;
}

/* Constructor */
Novel::Novel(const char *title,const char *author, int year, int quantity) {
    _title = new char[strlen(title) + 1];
    strcpy(_title,title);
    _author = new char[strlen(author) + 1];
    strcpy(_author,author);
    _year = year;
    _quantity = quantity;
}

/* Destructor */
Novel::~Novel(){
    delete[] _author;
    delete[] _title;
}

document_type Novel::getDocType(){
	return document_type::DOC_NOVEL;
}

void Novel::print(){
    printf("Novel, title: %s, author: %s, year: %d, quantity: %d\n", this->_title, this->_author, this->_year, this->_quantity);
}

void Novel::updateAuthor(const char *newAuthor){
    _author = new char[strlen(newAuthor) + 1];
    strcpy(_author,newAuthor);
}

char *Novel::getAuthor(){
    return _author;
}

/* Constructor */
Comic::Comic(const char *title, const char *author, int issue, int year, int quantity) {
    _title = new char[strlen(title) + 1];
    strcpy(_title,title);
    _author = new char[strlen(author) + 1];
    strcpy(_author,author);
    _issue = issue;
    _year = year;
    _quantity = quantity;
}

/* Destructor */
Comic::~Comic(){
    delete[] _author;
    delete[] _title;
}

document_type Comic::getDocType(){
	return document_type::DOC_COMIC;
}

void Comic::print(){
    printf("Comic, title: %s, author: %s, issue: %d, year: %d, quantity: %d\n", this->_title, this->_author, this->_issue, this->_year, this->_quantity);
}

void Comic::updateAuthor(const char *newAuthor){
    _author = new char[strlen(newAuthor) + 1];
    strcpy(_author, newAuthor);
}

void Comic::updateIssue(int newIssue){
    _issue = newIssue;
}

char *Comic::getAuthor(){
    return _author;
}

int Comic::getIssue(){
    return _issue;
}

/* Constructor */
Magazine::Magazine(const char *title, int issue, int year, int quantity) {
    _title = new char[strlen(title) + 1];
    strcpy(_title,title);
    _issue = issue;
    _year = year;
    _quantity = quantity;
}

/* Destructor */
Magazine::~Magazine(){
    delete[] _title;
}

document_type Magazine::getDocType(){
	return document_type::DOC_MAGAZINE;
}

void Magazine::print(){
    printf("Magazine, title: %s, issue: %d, year: %d, quantity: %d\n",this->_title, this->_issue, this->_year, this->_quantity);
}

void Magazine::updateIssue(int newIssue){
    _issue = newIssue;
}

int Magazine::getIssue(){
    return _issue;
}

/* Constructor */
Library::Library(){}

void Library::print(){
	for(int i=0;i<_docs.size();i++){
		_docs[i]->print();
	}
}


int Library::dumpCSV(const char *filename){
	char buff[100];
    char *_fn = new char[strlen(filename) + 1];
    strcpy(_fn,filename);
    int fd = open(_fn,O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
    if(fd == -1){
    	return 1;
    }
    for(int i=0;i<_docs.size();i++){

    	if(_docs[i]->getDocType() == DOC_NOVEL){
    		Novel* novelType;
    		novelType = dynamic_cast<Novel*>(_docs[i]);
    		char *title = new char[strlen(novelType->getTitle()) + 1];
   			strcpy(title, novelType->getTitle());
   			char *author = new char[strlen(novelType->getAuthor()) + 1];
   			strcpy(author, novelType->getAuthor());
   			int year = novelType->getYear();
    		int quantity = novelType->getQuantity();

    		sprintf(buff, "novel,%s,%s,,%d,%d\n",title,author,year,quantity);
   			write(fd,buff,strlen(buff));
    	}
    	else if(_docs[i]->getDocType() == DOC_COMIC){
    		Comic* comicType;
    		comicType = dynamic_cast<Comic*>(_docs[i]);
    		char *title = new char[strlen(comicType->getTitle()) + 1];
   			strcpy(title, comicType->getTitle());
   			char *author = new char[strlen(comicType->getAuthor()) + 1];
   			strcpy(author, comicType->getAuthor());
   			int issue = comicType->getIssue();
    		int year = comicType->getYear();
    		int quantity = comicType->getQuantity();

   			sprintf(buff,"comic,%s,%s,%d,%d,%d\n",title,author,issue,year,quantity);
   			write(fd,buff,strlen(buff));
    	}
    	else{
    		Magazine* magazineType;
    		magazineType = dynamic_cast<Magazine*>(_docs[i]);
    		char *title = new char[strlen(magazineType->getTitle()) + 1];
   			strcpy(title, magazineType->getTitle());
   			int issue = magazineType->getIssue();
   			int year = magazineType->getYear();
   			int quantity = magazineType->getQuantity();
    		sprintf(buff,"magazine,%s,,%d,%d,%d\n",title,issue,year,quantity);

    		write(fd,buff,strlen(buff));
    	}
    }
    close(fd);
    return 0;
}

Document *Library::searchDocument(const char *title){
    for(int i=0;i<_docs.size();i++){
    	if(strcmp(_docs[i]->getTitle(), title) == 0){
    		return _docs[i];
    	}
    }
    return NULL;
}

int Library::addDocument(Document *d){
    if(d != NULL && searchDocument(d->getTitle()) == NULL){
    	_docs.push_back(d);
    	return 0;
    }
    return 1;
}

int Library::delDocument(const char *title){
    for(int i=0; i<_docs.size(); i++){
    	if(strcmp(_docs[i] -> getTitle(), title) == 0){
    		_docs.erase(_docs.begin() + i);
    		return 0;
    	}
    }
    return 1;
}

int Library::countDocumentOfType(document_type t){
    int i=0;
    for(int j=0;j<_docs.size();j++){
    	if(_docs[j]->getDocType() == t){
    		i++;
    	}
    }
    return i;
}

int Library::borrowDoc(const char *title){
	for(int i=0;i < _docs.size();i++){
		if(strcmp(_docs[i]->getTitle(), title) == 0 && _docs[i]->getQuantity() > 0){
			_docs[i]->updateQuantity(_docs[i]->getQuantity() - 1);
			return 0;
		}
	}
	return 1;
}

int Library::returnDoc(const char *title){
	for(int i=0;i<_docs.size();i++){
		if(strcmp(_docs[i]->getTitle(), title) == 0){
			_docs[i]->updateQuantity(_docs[i]->getQuantity() + 1);
			return 0;
		}
	}
	return 1;
}