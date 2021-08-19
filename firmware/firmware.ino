#include "CrawlController.h"
CrawlController crawl;

void setup(){
    crawl.begin();
}

void loop(){
    crawl.update();
}