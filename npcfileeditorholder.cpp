#include "npcfileeditorholder.h"

NPCFileEditorHolder::NPCFileEditorHolder()
{
    
}

NPCFileEditorHolder::~NPCFileEditorHolder()
{
    clear();
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

int NPCFileEditorHolder::getNPCIndex(NPCFileEditorHolder::NPCHolder *npc) const
{
    unsigned int index = 0;
    for (std::vector < NPCHolder * >::iterator iterator = _npcs.begin();
         iterator != _npcs.end();
         iterator++, index++)
        if ((*iterator) == npc)
            return index;

    printf("[NPCFileEditorHolder::getNPCIndex] Error: Can't get index of npc with address: %d\n",
           npc);
    return -1;
}

