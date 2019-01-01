/**
 * @file DatasetAbstract.h
 * 
 * Clase abstracta que sigue el patrón de diseño "multiton". Esta clase estática
 * almacena los punteros a toda la información disponible que se haya extraido
 * del fichero de entrada y proporciona un acceso único a estos.
 * 
 * @author  Víctor Guillermo Andrés Escudero
 * @date    01/08/2018
 * @version 1.0
 * 
 **/

#ifndef DATASET_H
#define DATASET_H

#include <unordered_map>
#include <string>
#include <vector>

class DatasetAbstract /*: public std::enable_shared_from_this<Dataset>*/ {
public:
    DatasetAbstract( const std::string&, unsigned int);
    
    static DatasetAbstract* FactoryDataset (const std::string&, const std::string&, unsigned int);
    static DatasetAbstract* getDataset (const std::string&);
    
    virtual void getData (unsigned int index, std::vector<double>& ) = 0;
    
    virtual void addData (const double[], unsigned int) = 0;
    virtual void addData (const std::vector<double>&) = 0;
    virtual void modifyData (unsigned int, std::vector<double>&) = 0;
    
    virtual size_t size() = 0;
    virtual size_t getDataDimension (unsigned int) = 0;
    
    DatasetAbstract(const DatasetAbstract& orig);
    
    std::string getName();

    virtual ~DatasetAbstract() = 0;
private:
    
    static std::unordered_map<std::string, DatasetAbstract*> dataset_names; ///< Tabla hash donde se almacenan los conj. de datos.
    std::string name; ///< Nombre del conj. de datos.
    
    void addDataset (const std::string&);
    
    virtual void printDataset() = 0;
};

#endif /* DATASET_H */

