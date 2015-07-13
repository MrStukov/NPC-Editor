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
        unsigned int dialogId;
    };

    /*
     * Структура, описывающая ответ на вопрос
     */
    struct DialogNode;

    struct DialogAnswer
    {
        enum Type
        {
            Type_Blank,
            Type_Shop
        };

        unsigned int destinationId;
        DialogNode *destination;
        Type answerType;
        std::string text;
    };

    /*
     * Структура, описывающая одну ноду диалога
     */
    struct DialogNode
    {
        DialogNode( )
        {

        }

        unsigned int dialogId;
        unsigned int id;

        std::string text;
        std::vector < DialogAnswer > answers;
    };

    /*
     * Структура, хранящая все данные о диалоге.
     */
    struct DialogHolder
    {
        DialogHolder ( unsigned int enterId=0,
                       DialogNode *enterDialogRoot=nullptr)
        {
            id = enterId;
            root = enterDialogRoot;
        }

        unsigned int id;
        std::string name;

        DialogNode *root;
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
    unsigned int npcLength() const;
    
    // Получение данных о конкретном NPC.
    NPCHolder *getNPC( unsigned int index ) const;
    
    // Сохранение файла
    bool save( const std::string &path = "");

    // Получение индекса по NPC
    int getNPCIndex( NPCHolder *npc ) const;

    // Получение количества диалогов в файле
    unsigned int dialogLength() const;

    // Получение диалога по индексу
    DialogHolder* getDialog( unsigned int index ) const;

    // Получение ноды по номеру диалога и номеру ноды
    DialogNode *findNode( unsigned int rootId, unsigned int nodeId );
private:
    // Приватные методы для открытия файла
    std::string openingFile( const std::string &path ) const;
    std::string decodeFile( const std::string &fileData ) const;
    bool parseData ( const std::string &data );

    bool parseNPCNode ( tinyxml2::XMLElement *npcElement );
    bool parseDialogRoot ( tinyxml2::XMLElement *dialogElement );

    std::string parseDialogNodeText( tinyxml2::XMLElement *dialogNode );
    void parseAnswers( DialogNode *dialog, tinyxml2::XMLElement *dialogNode );


    std::string _currentOpenPath;
    bool _isOpen;

    std::vector < NPCHolder * > _npcs;
    std::vector < DialogHolder * > _dialogues;
    std::vector < DialogNode * > _nodesHolder;
};

#endif // NPCFILEEDITORHOLDER_H
