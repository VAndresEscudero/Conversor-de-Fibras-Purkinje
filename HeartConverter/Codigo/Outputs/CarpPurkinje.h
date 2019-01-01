/**
 * @file CarpPurkinje.h
 * 
 * Clase que representa un fichero de fibras de Purkinje en CARP.
 * 
 * @author  Víctor Guillermo Andrés Escudero
 * @date    01/08/2018
 * @version 1.0
 * 
 **/

#ifndef CARPPURKINJE_H
#define CARPPURKINJE_H

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include "../Datasets/DatasetAbstract.h"

#include "AbstractFile.h"

class CarpPurkinje : public AbstractFile{
public:

    CarpPurkinje (const std::string&);
    
    void print(std::ostream&) const;    
    
private:
    
    typedef struct MyHash {
        size_t operator()(const std::vector<double> &val) const {
            size_t x = std::hash<double>()(val[0]);
            size_t y = std::hash<double>()(val[1]);
            size_t z = std::hash<double>()(val[2]);
            return ((x ^ (y << 1)) ^ (z << 2) );
        }
    } MyHash;
    
    void calcRelations();
    void setSons(unsigned int, unsigned int);
    void setFathers(unsigned int, unsigned int);
    void setAttributesValues();
    void createDefaultConfigFile();

    size_t createNewPoint(std::vector<double>&, std::vector<double>&, float);
    void addRelations (unsigned int, unsigned int, unsigned int);
    void eraseRelation (std::unordered_multimap< std::vector<double> , unsigned int, MyHash>&, std::vector<double>, unsigned int);
    void removeExtraSon(std::vector<double>, unsigned int);
    size_t createNewElement(unsigned int, unsigned int, unsigned int);
    size_t modifyRelations(unsigned int, unsigned int, unsigned int);
    void removeExtraRelations ();
    
    void printRelations(std::ostream&, std::vector<double>) const;
    
    void printSeveralParents();
    
    
    DatasetAbstract* points;        ///< Puntero a las coordenadas de los puntos.
    DatasetAbstract* elements;      ///< Puntero a los índices de los puntos que componen cada elemento.
    //DatasetAbstract* previous_pts;
    
    typedef struct PurkinjeRelations {
        signed long parents[2] {-1, -1};
        signed long sons[2] {-1, -1};
    } PurkinjeRelations;
    
    const std::string config_name = "config.cfg";
    unsigned int cable_size = 75;
    double gap_resistance = 100;
    double conductivity = 0.0006;
 
    
    std::unordered_multimap< std::vector<double> , unsigned int, MyHash> searchSons;
    std::unordered_multimap< std::vector<double> , unsigned int, MyHash> searchParents;
    
    const unsigned int MAX_SONS = 2;
    const unsigned int MAX_PARENTS = 2;

};





#endif /* CARPPURKINJE_H */

