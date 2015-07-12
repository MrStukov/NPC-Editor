#include "npcfileeditorholder.h"

NPCFileEditorHolder::NPCFileEditorHolder()
{
    
}

NPCFileEditorHolder::~NPCFileEditorHolder()
{
    clear();
}

bool NPCFileEditorHolder::openFile(const std::string &file)
{
    // Очищаем прошлый файл
    clear();

    // Открываем новый файл
    std::string fileData = openingFile( file );
    if (fileData.length() <= 0)
    {
        printf("[NPCFileEditorHolder::openFile] Error: Can't load file.\n");
        return false;
    }

    std::string decodedData = decodeFile( fileData );
    if (decodedData.length() <= 0)
    {
        printf("[NPCFileEditorHolder::openFile] Error: Can't open file.\n");
        return false;
    }

    bool parseResult = parseData( decodedData );
    if (!parseResult)
        printf("[NPCFileEditorHolder::openFile] Error: Can't parse file.\n");
    else
    {
        _currentOpenPath = file;
        _isOpen = true;
    }

    return parseResult;
}

bool NPCFileEditorHolder::isOpen() const
{
    return _isOpen;
}

void NPCFileEditorHolder::clear()
{
    for (std::vector < NPCHolder * >::iterator iterator = _npcs.begin();
         iterator != _npcs.end();
         iterator++)
        delete (*iterator);
    _npcs.clear();
    _isOpen = false;
}

NPCFileEditorHolder::NPCHolder *NPCFileEditorHolder::createNPC(unsigned int id)
{
    // Проврка на наличие предоставленного индекса
    // ( id == 0 -> индекс предоставлен не был )
    unsigned int resultId = 0;
    if (id == 0)
    {
        // Поиск максимального id
        unsigned int maxId = 0;
        for (std::vector < NPCHolder *>::iterator iterator = _npcs.begin();
             iterator != _npcs.end();
             iterator++)
            if ( (*iterator)->id > maxId )
                maxId = (*iterator)->id;

        resultId = maxId + 1;
    }
    else
    {
        // Проверка на существование предоставленного индекса
        bool found = false;
        for (std::vector < NPCHolder * >::iterator iterator = _npcs.begin();
             iterator != _npcs.end() && !found;
             iterator++)
            if ( id == (*iterator)->id )
                found = true;

        if (found)
        {
            printf("[NPCFileEditorHolder::createNPC] Error: Can't create npc with id %d\n", id);
            return nullptr;
        }

        resultId = id;
    }

    NPCHolder *newItem = new NPCHolder( resultId );

    _npcs.push_back( newItem );

    return newItem;
}

void NPCFileEditorHolder::removeNPC(unsigned int id)
{
    NPCHolder *npc = nullptr;
    for (std::vector < NPCHolder * >::iterator iterator = _npcs.begin();
         iterator != _npcs.end();
         iterator++)
        if ( id == (*iterator)->id )
        {
            delete (*iterator);
            npc = (*iterator);
        }

    _npcs.erase( std::remove( _npcs.begin(), _npcs.end(), npc ) );
}

unsigned int NPCFileEditorHolder::length() const
{
    return _npcs.size();
}

NPCFileEditorHolder::NPCHolder *NPCFileEditorHolder::getNPC(unsigned int index) const
{
    if (index >= _npcs.size())
    {
        printf("[NPCFileEditorHolder::getNPC] Error: Can't get NPC with %d index.\n", index);
        return nullptr;
    }

    return _npcs[index];
}

bool NPCFileEditorHolder::save(const std::string &path)
{
    // Открытие файла
    if (path.length() <= 0 &&
        _currentOpenPath.length() <= 0)
    {
        printf("[NPCFileEditorHolder::save] Error: Wrong method usage. No path provided.\n");
        return false;
    }

//    std::ofstream file;

//    if (path.length() > 0)
    //        file.open( path );
    //    else
    //        file.open( _currentOpenPath  );

//    if (!file.is_open())
//        return false;

    // Открываем файл в стиле C, потому что tinyxml работает только с таким видом
    FILE *filePointer = nullptr;
    if (path.length() > 0)
        filePointer = fopen( path.c_str(), "w" );
    else
        filePointer = fopen( _currentOpenPath.c_str(), "w" );

    // Запись в файл
    tinyxml2::XMLPrinter printer(filePointer);

    printer.OpenElement("npcs");

    for (std::vector < NPCHolder * >::const_iterator iterator = _npcs.begin();
         iterator != _npcs.end();
         iterator++)
    {
        printer.OpenElement("npc");

        printer.PushAttribute("id", std::to_string((*iterator)->id).c_str() );
        printer.PushAttribute("name", (*iterator)->name.c_str());

        printer.CloseElement(false);
    }

    printer.CloseElement(false);

    fclose(filePointer);

    return true;
}

int NPCFileEditorHolder::getNPCIndex(NPCFileEditorHolder::NPCHolder *npc) const
{
    unsigned int index = 0;
    for (std::vector < NPCHolder * >::const_iterator iterator = _npcs.begin();
         iterator != _npcs.end();
         iterator++, index++)
        if ((*iterator) == npc)
            return index;

    printf("[NPCFileEditorHolder::getNPCIndex] Error: Can't get index of npc.\n");
    return -1;
}

std::string NPCFileEditorHolder::openingFile(const std::string &path) const
{
    std::fstream file( path );
    if (!file.is_open())
    {
        printf("[NPCFileEditorHolder::openingFile] Error: Can't open file. Path: %s\n",
               path.c_str());
        return std::string();
    }

    return std::string((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
}

std::string NPCFileEditorHolder::decodeFile(const std::string &fileData) const
{
    return fileData;
}

bool NPCFileEditorHolder::parseData(const std::string &data)
{
    tinyxml2::XMLDocument document;
    tinyxml2::XMLError error = document.Parse( data.c_str(), data.length() );
    if (error != tinyxml2::XML_NO_ERROR)
    {
        printf("[NPCFileEditorHolder::parseData] Error: Can't parse decoded file data.\n");
        return false;
    }

    // Получение корневой ноды
    tinyxml2::XMLElement *rootElement = document.FirstChildElement("npcs");
    if (!rootElement)
    {
        printf("[NPCFileEditorHolder::parseData] Error: Can't find root element.\n");
        return false;
    }

    // Обработка
    tinyxml2::XMLElement *npcElement = rootElement->FirstChildElement("npc");
    while (npcElement)
    {
        if (!parseNode( npcElement ))
            printf("[NPCFileEditorHolder::parseData] Warning: Can't parse node.\n");
        npcElement = npcElement->NextSiblingElement("npc");
    }

    return true;
}

bool NPCFileEditorHolder::parseNode(tinyxml2::XMLElement *npcElement)
{
    const char *id = npcElement->Attribute("id");
    const char *name = npcElement->Attribute("name");

    if (!id || !name)
        return false;

    NPCHolder *npc = new NPCHolder(
        atoi( id ),
        std::string( name )
    );

    _npcs.push_back( npc );

    return true;
}

