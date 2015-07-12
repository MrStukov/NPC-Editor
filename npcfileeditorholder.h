#ifndef NPCFILEEDITORHOLDER_H
#define NPCFILEEDITORHOLDER_H

#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "Libraries/Tinyxml2/tinyxml2.h"

/*
 * Класс, управляющий редактированием 
 * файла с NPC.
 */
class NPCFileEditorHolder
{
public:
    /*
     * Структура, хранящая все данные о NPC.
     * ( позиции тут нет, потому что она передается из карты )
     */
    struct NPCHolder
    {
        NPCHolder( unsigned int enterId=0,
                   const std::string &enterName = "undefined")
        {        
            id = enterId;
            name = enterName;
        }

        unsigned int id;
        std::string name;
    };
    
    // Конструктор и деструктор
    NPCFileEditorHolder();
    ~NPCFileEditorHolder();
    
    // Метод открытия файла
    bool openFile( const std::string &file );
    
    // Открыт ли файл?
    bool isOpen() const;
    
    // Если файл открыт, производит очистку выделенной памяти
    void clear();
    
    // Создание нового NPC.
    NPCHolder *createNPC( unsigned int id=0 );
    
    // Удаление NPC
    void removeNPC( unsigned int id=0 );

    // Получение количества NPC в файле.
    unsigned int length() const;
    
    // Получение данных о конкретном NPC.
    NPCHolder *getNPC( unsigned int index ) const;
    
    // Сохранение файла
    bool save( const std::string &path = "");

    // Получение индекса по NPC
    int getNPCIndex( NPCHolder *npc ) const;

private:
    // Приватные методы для открытия файла
    std::string openingFile( const std::string &path ) const;
    std::string decodeFile( const std::string &fileData ) const;
    bool parseData ( const std::string &data );

    bool parseNode ( tinyxml2::XMLElement *npcElement );

    std::string _currentOpenPath;
    bool _isOpen;
    std::vector < NPCHolder * > _npcs;
};

#endif // NPCFILEEDITORHOLDER_H
