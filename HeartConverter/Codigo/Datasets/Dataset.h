/**
 * @file Dataset.h
 * 
 * Clase concreta donde se almacena la información. Se trata de un vector de
 * vectores y varias funciones auxiliares para añadir y extraer información de
 * este.
 * 
 * @tparam T    El tipo de datos que almacenará el vector.
 * 
 * @author  Víctor Guillermo Andrés Escudero
 * @date    01/08/2018
 * @version 1.0
 * 
 **/

#ifndef DATASETDOUBLE_H
#define DATASETDOUBLE_H

#include "DatasetAbstract.h"
#include <string>
#include <vector>
#include <iostream>

template <typename T>
class Dataset : public DatasetAbstract{
public:
    typedef T value_type; ///< Variable que contine el tipo de dato usado.
    
    /**
     * Constructor. Llama al constructor de la clase de la que hereda para
     * añadir el nuevo conj. de datos a la tabla hash. También reserva en el
     * vector espacio suficiente para la cantidad de elementos que se le indique.
     * No debe de ser llamado por si solo, en su lugar este constructor se llamará
     * a traves del método factoría de su clase padre.
     * 
     * @param [in]  size    Número de elementos a reservar en el vector. 0 por defecto.
     **/
    Dataset(const std::string& name, unsigned int size = 0) : DatasetAbstract(name, size) {
        if (size > 0){
            this->data.reserve(size);
        }
    }
    
    
    /**
     * Añade al final del vector el array suministrado.
     * 
     * @param [in]  data    Puntero al array.
     * @param [in]  size    Tamaño del array.
     **/
    void addData (const double data[], unsigned int size) {
        std::vector<T> data_new;

        for (int i = 0; i < size; ++i) {
            data_new.push_back(data[i]);
        }
        this->data.push_back(data_new);
    }
    
    /**
     * Añade al final del vector el vector suministrado.
     * 
     * @param [in]  data    Vector a añadir.
     **/    
    void addData (const std::vector<double>& data) {
        std::vector<T> aux (data.begin(), data.end());
        this->data.push_back(aux);
    }
    
    /**
     * Obtiene el vector que se encuentra en la posición deseada.
     * 
     * @param [in]  index   Índice del vector a obtener.
     * @param [out] data    Vector vacio donde se almacena el resultado.
     **/
    void getData (unsigned int index, std::vector<double>& data) {
        data = std::vector<double> (this->data[index].begin(), this->data[index].end());
    }
    
    /**
     * Modifica los valores del elemento deseado.
     * @param [in] index    Índice del vector a modificar
     * @param [in] new_data     Vector que sustituye al vector original
     **/
    void modifyData (unsigned int index, std::vector<double>& new_data) {
        data[index].clear();
        
        std::vector<T> aux (new_data.begin(), new_data.end());
        this->data[index] = aux;
    }
    
    /**
     * Devuelve la cantidad de elementos que el vector de vectores tiene.
     **/
    size_t size() {
        return data.size();
    }
    
    /**
     * Devuelve la cantidad de elementos que el vector en la posición deseada
     * tiene.
     * Si el índice es mayor al número de elementos se muestra un mensaje de
     * error y se devuelve 0.
     * 
     * @param [in]  element Índice del vector a obtener su tamaño.
     * @return El numero de elementos que contiene el vector deseado, 0 si no se
     *         encuentra.
     **/
    size_t getDataDimension(unsigned int element) {
        if (element >= data.size()){
            std::cout << "No se ha encontrado el elemento numero " << element << "." << std::endl;
            return 0;
        }
        else
            return data[element].size();
    }
    
    
    Dataset(const Dataset& orig) {};
    virtual ~Dataset() {}
private:
    std::vector<std::vector<T>> data; ///< Vector de vectores con la información.

    /**
     * Función de ayuda para comprobar los datos almacenados.
     **/
    void printDataset() {
        for (auto vector : data) {
            for (auto num : vector) {
                std::cout << num << ", ";
            }
            std::cout << std::endl;

        }

        std::cout << getName() << std::endl;
    }

};

#endif /* DATASETDOUBLE_H */

