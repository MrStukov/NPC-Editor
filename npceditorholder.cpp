#include "npceditorholder.h"

// Обрезать с начала
static inline std::string &ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// Обрезать с конца
static inline std::string &rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// Обрезать с обоих сторон
static inline std::string &trim(std::string &s)
{
    return ltrim(rtrim(s));
}

NPCEditorHolder::NPCEditorHolder()
{

}

NPCEditorHolder::~NPCEditorHolder()
{

}

bool NPCEditorHolder::openFile(const std::string &file)
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

bool NPCEditorHolder::isOpen() const
{
    return _isOpen;
}

void NPCEditorHolder::clear()
{
    for (std::vector < NPCHolder * >::iterator iterator = _npcs.begin();
         iterator != _npcs.end();
         iterator++)
        delete (*iterator);
    _npcs.clear();

    for (std::vector < DialogHolder * >::iterator iterator = _dialogues.begin();
         iterator != _dialogues.end();
         iterator++)
        delete (*iterator);
    _dialogues.clear();

    for (std::vector < DialogNode *>::iterator iterator = _nodesHolder.begin();
         iterator != _nodesHolder.end();
         iterator++)
        delete (*iterator);
    _nodesHolder.clear();

    _isOpen = false;
}

NPCEditorHolder::NPCHolder *NPCEditorHolder::createNPC(unsigned int id)
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

void NPCEditorHolder::removeNPC(unsigned int id)
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

unsigned int NPCEditorHolder::npcLength() const
{
    return _npcs.size();
}

NPCEditorHolder::NPCHolder *NPCEditorHolder::getNPC(unsigned int index) const
{
    if (index >= _npcs.size())
    {
        printf("[NPCFileEditorHolder::getNPC] Error: Can't get NPC with %d index.\n", index);
        return nullptr;
    }

    return _npcs[index];
}

bool NPCEditorHolder::save(const std::string &path)
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

    // Запись в файл npc
    tinyxml2::XMLPrinter printer(filePointer);

    printer.OpenElement("npcs");

    for (std::vector < NPCHolder * >::const_iterator iterator = _npcs.begin();
         iterator != _npcs.end();
         iterator++)
    {
        printer.OpenElement("npc");

        printer.PushAttribute("id", std::to_string((*iterator)->id).c_str() );
        printer.PushAttribute("name", (*iterator)->name.c_str());
        printer.PushAttribute("dialog", std::to_string((*iterator)->dialogId).c_str());

        printer.CloseElement(false);
    }

    printer.CloseElement(false);

    // Запись в файл диалогов

    printer.OpenElement("dialogues");

    for (std::vector < DialogHolder *>::const_iterator iterator = _dialogues.begin();
         iterator != _dialogues.end();
         iterator++)
    {
        printer.OpenElement("dialogRoot");

        printer.PushAttribute("id", (*iterator)->id);
        printer.PushAttribute("starts", (*iterator)->startId);
        printer.PushAttribute("name", (*iterator)->name.c_str());

        for (std::vector < DialogNode * >::const_iterator dialogIterator = _nodesHolder.begin();
             dialogIterator != _nodesHolder.end();
             dialogIterator++)
            if ((*dialogIterator)->dialogId == (*iterator)->id)
            {
                printer.OpenElement("dialogNode");
                printer.PushAttribute("id", std::to_string((*dialogIterator)->id).c_str());

                // Записываем текст
                printer.OpenElement("text");

                printer.PushText( (*dialogIterator)->text.c_str() );

                printer.CloseElement(false);

                // Записываем ответы
                for (std::vector < DialogAnswer >::const_iterator answerIterator = (*dialogIterator)->answers.begin();
                     answerIterator != (*dialogIterator)->answers.end();
                     answerIterator++)
                {
                    printer.OpenElement("answer");

                    printer.PushAttribute("id", std::to_string(answerIterator->id).c_str());
                    switch (answerIterator->answerType)
                    {
                    case DialogAnswer::Type_Blank:
                        printer.PushAttribute("type", "blank");
                        printer.PushAttribute("goto", std::to_string(answerIterator->destinationId).c_str());
                        break;
                    case DialogAnswer::Type_Shop:
                        printer.PushAttribute("type", "shop");
                        // TODO: Добавить поддержку магазинов
                        break;
                    }

                    printer.PushText(answerIterator->text.c_str());

                    printer.CloseElement(false);
                }

                printer.CloseElement(false);
            }

        printer.CloseElement(false);
    }

    printer.CloseElement(false);

    fclose(filePointer);

    return true;
}

int NPCEditorHolder::getNPCIndex(NPCEditorHolder::NPCHolder *npc) const
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

unsigned int NPCEditorHolder::dialogLength() const
{
    return _dialogues.size();
}

NPCEditorHolder::DialogHolder *NPCEditorHolder::getDialog(unsigned int index) const
{
    if (index >= _dialogues.size())
    {
        printf("[NPCFileEditorHolder::getDialog] Error: Can't get %d dialog. Out of size.\n",
               index);
        return nullptr;
    }

    return _dialogues[index];
}

NPCEditorHolder::DialogNode *NPCEditorHolder::getNode(unsigned int dialogId, unsigned int nodeId)
{
    for (std::vector < DialogNode * >::iterator iterator = _nodesHolder.begin();
         iterator != _nodesHolder.end();
         iterator++)
        if ((*iterator)->dialogId == dialogId &&
            (*iterator)->id == nodeId)
            return (*iterator);

    return nullptr;
}

unsigned int NPCEditorHolder::rootLength(unsigned int dialogId)
{
    unsigned int n = 0;
    for (std::vector < DialogNode * >::const_iterator iterator = _nodesHolder.begin();
         iterator != _nodesHolder.end();
         iterator++)
        if ((*iterator)->dialogId == dialogId)
            n++;

    return n;
}

NPCEditorHolder::DialogNode *NPCEditorHolder::getNodeByIndex(unsigned int dialogId, unsigned int nodeIndex)
{
    unsigned int n = 0;
    for (std::vector < DialogNode * >::const_iterator iterator = _nodesHolder.begin();
         iterator != _nodesHolder.end();
         iterator++)
    {
        if ((*iterator)->dialogId == dialogId)
        {
            if (n != nodeIndex)
            {
                n++;
            }
            else
            {
                return (*iterator);
            }
        }
    }

    printf("[NPCEditorHolder::getNodeByIndex] Error: Can't find node with index %d.\n",
           nodeIndex);

    return nullptr;
}

std::string NPCEditorHolder::openingFile(const std::string &path) const
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

std::string NPCEditorHolder::decodeFile(const std::string &fileData) const
{
    return fileData;
}

bool NPCEditorHolder::parseData(const std::string &data)
{
    tinyxml2::XMLDocument document;
    tinyxml2::XMLError error = document.Parse( data.c_str(), data.length() );
    if (error != tinyxml2::XML_NO_ERROR)
    {
        printf("[NPCFileEditorHolder::parseData] Error: Can't parse decoded file data.\n");
        return false;
    }

    // Получение корневой ноды
    tinyxml2::XMLElement *npcRootElement = document.FirstChildElement("npcs");
    if (!npcRootElement)
    {
        printf("[NPCFileEditorHolder::parseData] Error: Can't find npc root element.\n");
        return false;
    }

    // Обработка npc
    tinyxml2::XMLElement *npcElement = npcRootElement->FirstChildElement("npc");
    while (npcElement)
    {
        if (!parseNPCNode( npcElement ))
            printf("[NPCFileEditorHolder::parseData] Warning: Can't parse npc node.\n");

        npcElement = npcElement->NextSiblingElement("npc");
    }

    // Получение корневой ноды
    tinyxml2::XMLElement *dialogRootElement = document.FirstChildElement("dialogues");
    if (!dialogRootElement)
    {
        printf("[NPCFileEditorHolder::parseData] Error: Can't find dialog root element.\n");
        return false;
    }

    // Обработка диалогов
    tinyxml2::XMLElement *dialogElement = dialogRootElement->FirstChildElement("dialogRoot");
    while (dialogElement)
    {
        if (!parseDialogRoot( dialogElement ))
            printf("[NPCFileEditorHolder::parseData] Warning: Can't parse dialog node.\n");

        dialogElement = dialogElement->NextSiblingElement("dialogRoot");
    }

    return true;
}

bool NPCEditorHolder::parseNPCNode(tinyxml2::XMLElement *npcElement)
{
    const char *id = npcElement->Attribute("id");
    const char *name = npcElement->Attribute("name");
    const char *dialogId = npcElement->Attribute("dialog");

    if (!id || !name || !dialogId)
        return false;

    NPCHolder *npc = new NPCHolder();

    npc->id = atoi(id);
    npc->name = name;
    npc->dialogId = atoi(dialogId);

    _npcs.push_back( npc );

    return true;
}

bool NPCEditorHolder::parseDialogRoot(tinyxml2::XMLElement *dialogElement)
{
    const char *dialogId = dialogElement->Attribute("id");
    const char *startId = dialogElement->Attribute("starts");

    if (!dialogId)
    {
        printf("[NPCFileEditorHolder::parseDialogNode] Error: Can't find dialog root id.\n");
        return false;
    }

    if (!startId)
    {
        printf("[NPCFileEditorHolder::parseDialogNode] Error: Can't find start dialog id.\n");
        return false;
    }

    DialogHolder *dialog = new DialogHolder( atoi(dialogId) );
    dialog->name = std::string( dialogElement->Attribute("name") );
    dialog->startId = (unsigned int) atoi(startId);

    // Обработка dialogNode
    tinyxml2::XMLElement *dialogNodeElement = dialogElement->FirstChildElement("dialogNode");
    while (dialogNodeElement)
    {
        DialogNode *dialogNode = new DialogNode();

        dialogNode->text = parseDialogNodeText( dialogNodeElement );
        dialogNode->id = atoi( dialogNodeElement->Attribute("id") );
        dialogNode->dialogId = dialog->id;

        parseAnswers( dialogNode, dialogNodeElement );

        _nodesHolder.push_back( dialogNode );

        dialogNodeElement = dialogNodeElement->NextSiblingElement("dialogNode");
    }

    // Доработка "ответов", так как все ноды получены
//    for (std::vector < DialogNode * >::iterator iterator = _nodesHolder.begin();
//         iterator != _nodesHolder.end();
//         iterator++)
//        if ((*iterator)->dialogId == dialog->id)
//            for (std::vector < DialogAnswer >::iterator answerIterator = (*iterator)->answers.begin();
//                 answerIterator != (*iterator)->answers.end();
//                 answerIterator++)
//                (*answerIterator).destination = findNode((*iterator)->dialogId, (*answerIterator).destinationId);

    _dialogues.push_back( dialog );

    return true;
}

std::string NPCEditorHolder::parseDialogNodeText(tinyxml2::XMLElement *dialogNode)
{
    std::string result;

    tinyxml2::XMLElement *textElement = dialogNode->FirstChildElement( "text" );
    if (!textElement)
        return result;

    result = std::string(textElement->GetText());

    return trim(result);
}

void NPCEditorHolder::parseAnswers(NPCEditorHolder::DialogNode *dialog, tinyxml2::XMLElement *dialogNode)
{
    tinyxml2::XMLElement *answerElement = dialogNode->FirstChildElement("answer");
    while (answerElement)
    {
        DialogAnswer answer;

        answer.text = std::string ( answerElement->GetText() );
        trim( answer.text );

        answer.id = atoi( answerElement->Attribute("id") );

        const char *answerType = answerElement->Attribute("type");
        if (strcmp(answerType, "blank") == 0)
        {
            answer.answerType = DialogAnswer::Type_Blank;
            answer.destinationId = atoi(answerElement->Attribute("goto"));
        }
        else if (strcmp(answerType, "shop") == 0)
        {
            answer.answerType = DialogAnswer::Type_Shop;
        }

        dialog->answers.push_back(answer);

        answerElement = answerElement->NextSiblingElement("answer");
    }
}

