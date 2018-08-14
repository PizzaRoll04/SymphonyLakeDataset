#ifndef CSV_FUNCTIONS_H
#define CSV_FUNCTIONS_H

#include <stdio.h>
#include <vector>
#include <map>
#include <set>
#include <string>


namespace symphony_lake_dataset {
    template <class I,class C> 
        bool loadStructuresFromCSV(const std::string & filename, std::map<I,C> & out, bool clear=true,
                std::vector<std::string> * ignored_lines=NULL) {
            printf("Opening file %s\n",filename.c_str());
            FILE * fp = fopen(filename.c_str(),"r");
            if (!fp) {
                perror(("Opening file "+filename+": ").c_str());
                return false;
            }
            if (clear) out.clear();
            while (!feof(fp)) {
                char line[1024]="";
                if (fgets(line,1024,fp)==NULL) {
                    continue;
                }
                if (line[0]=='%') {
                    if (ignored_lines) {
                        ignored_lines->push_back(line);
                    }
                    continue;
                }
                C data;
                I idx;
                if (loadFromString(line, idx, data)) {
                    out[idx] = data;
                }
            }
            fclose(fp);
            return true;
        }

    template <class C> 
        bool loadClassesFromCSV(const std::string & filename, std::vector<C> & out, bool clear=true,
                std::vector<std::string> * ignored_lines=NULL) {
            printf("Opening file %s\n",filename.c_str());
            FILE * fp = fopen(filename.c_str(),"r");
            if (!fp) {
                perror(("Opening file "+filename+": ").c_str());
                return false;
            }
            if (clear) out.clear();
            while (!feof(fp)) {
                char line[1024]="";
                if (fgets(line,1024,fp)==NULL) {
                    continue;
                }
                if (line[0]=='%') {
                    if (ignored_lines) {
                        ignored_lines->push_back(line);
                    }
                    continue;
                }
                C data;
                if (data.loadFromString(line)) {
                    out.push_back(data);
                }
            }
            fclose(fp);
            return true;
        }

    template <class C> 
        bool loadStructuresFromCSV(const std::string & filename, std::vector<C> & out, bool clear=true,
                std::vector<std::string> * ignored_lines=NULL) {
            printf("Opening file %s\n",filename.c_str());
            FILE * fp = fopen(filename.c_str(),"r");
            if (!fp) {
                perror(("Opening file "+filename+": ").c_str());
                return false;
            }
            if (clear) out.clear();
            while (!feof(fp)) {
                char line[1024]="";
                if (fgets(line,1024,fp)==NULL) {
                    continue;
                }
                if (line[0]=='%') {
                    if (ignored_lines) {
                        ignored_lines->push_back(line);
                    }
                    continue;
                }
                C data;
                if (loadFromString(line,data)) {
                    out.push_back(data);
                }
            }
            fclose(fp);
            return true;
        }


    template <class C> 
        bool loadStructuresFromCSV(const std::string & filename, std::set<C> & out, bool clear=true,
                std::vector<std::string> * ignored_lines=NULL) {
            printf("Opening file %s\n",filename.c_str());
            FILE * fp = fopen(filename.c_str(),"r");
            if (!fp) {
                perror(("Opening file "+filename+": ").c_str());
                return false;
            }
            if (clear) out.clear();
            while (!feof(fp)) {
                char line[1024]="";
                if (fgets(line,1024,fp)==NULL) {
                    continue;
                }
                if (line[0]=='%') {
                    if (ignored_lines) {
                        ignored_lines->push_back(line);
                    }
                    continue;
                }
                C data;
                if (loadFromString(line,data)) {
                    out.insert(data);
                }
            }
            fclose(fp);
            return true;
        }

    template <class C> 
        bool loadClassesFromCSV(const std::string & filename, std::set<C> & out, bool clear=true,
                std::vector<std::string> * ignored_lines=NULL) {
            printf("Opening file %s\n",filename.c_str());
            FILE * fp = fopen(filename.c_str(),"r");
            if (!fp) {
                perror(("Opening file "+filename+": ").c_str());
                return false;
            }
            if (clear) out.clear();
            while (!feof(fp)) {
                char line[1024]="";
                if (fgets(line,1024,fp)==NULL) {
                    continue;
                }
                if (line[0]=='%') {
                    if (ignored_lines) {
                        ignored_lines->push_back(line);
                    }
                    continue;
                }
                C data;
                if (data.loadFromString(line)) {
                    out.insert(data);
                }
            }
            fclose(fp);
            return true;
        }


};


#endif // CSV_FUNCTIONS_H
