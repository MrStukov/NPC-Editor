#ifndef NPCEDITORHOLDER_H
#define NPCEDITORHOLDER_H

#include <string>
#include <Libraries/Tinyxml2/tinyxml2.h>
#include <vector>
#include <algorithm>
#include <fstream>

/*
 * Класс, управляющий редактированием
 * файла с NPC
 */
class NPCEditorHolder
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
     * Структура, описывающая условие присутствия ответа
     */
    struct ExistCondition
    {
        enum Type
        {
            Type_None,
            Type_Complete,
            Type_Have
        };

        ExistCondition()
        {
            type = Type_None;
            conditionId = 0;
        }

        static std::string typeToString( Type type );
        static Type stringToType( const std::string &str );

        Type type;
        unsigned int conditionId;
    };

    /*
     * Структура, описывающая ответ на вопрос
     */
    struct DialogAnswer
    {
        enum Type
        {
            Type_Blank,
            Type_Shop
        };
        unsigned int id;

        unsigned int destinationId;
        Type answerType;
        std::string text;

        std::vector < ExistCondition > conditions;
    };

    /*
     * Структура, описывающая одну ноду диалога
     */
    struct DialogNode
    {
        DialogNode( )
        {
            dialogId = 0;
            id = 0;
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
                       unsigned int enterStartId=0)
        {
            id = enterId;
            startId = enterStartId;
        }

        unsigned int id;
        unsigned int startId;
        std::string name;
    };

    // Конструктор и деструктор
    NPCEditorHolder();
    ~NPCEditorHolder();

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

    // Создание нового диалога и получение указателя на него
    DialogHolder* createDialog( unsigned int id=0 );

    // Удаление диалога по id
    void removeDialog( unsigned int id=0 );

    // Создание новой ноды
    DialogNode *createNode( unsigned int dialogId, unsigned int id=0 );

    // Получение ноды по номеру диалога и номеру ноды
    DialogNode *getNode (unsigned int dialogId, unsigned int nodeId );

    // Получение количества нод по номеру диалога
    unsigned int rootLength ( unsigned int dialogId );

    // Получение ноды по ее индексу
    DialogNode *getNodeByIndex (unsigned int dialogId, unsigned int nodeIndex );

private:
    // Приватные методы для открытия файла
    std::string openingFile( const std::string &path ) const;
    std::string decodeFile (const std::string &fileData ) const;
    bool parseData (const std::string &data );

    bool parseNPCNode( tinyxml2::XMLElement *npcElement );
    bool parseDialogRoot ( tinyxml2::XMLElement *dialogElemenet );

    std::string parseDialogNodeText (tinyxml2::XMLElement *dialogNode );
    void parseAnswers ( DialogNode *dialog, tinyxml2::XMLElement *dialogNode );


    std::string _currentOpenPath;
    bool _isOpen;

    std::vector < NPCHolder * > _npcs;
    std::vector < DialogHolder * > _dialogues;
    std::vector < DialogNode * > _nodesHolder;
};

#endif // NPCEDITORHOLDER_H
