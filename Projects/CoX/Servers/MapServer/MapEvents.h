/*
 * Super Entity Game Server Project
 * http://segs.sf.net/
 * Copyright (c) 2006 - 2016 Super Entity Game Server Team (see Authors.txt)
 * This software is licensed! (See License.txt for details)
 *

 */

#pragma once
#include "LinkLevelEvent.h"
#include "BitStream.h"
#include "CRUD_Link.h"
#include "CRUD_Events.h"
#include "MapLink.h"

#include <glm/vec3.hpp>
#include <QtCore/QString>

class Entity;
typedef CRUDLink_Event MapLinkEvent; //<MapLink>

class MapEventTypes : public CRUD_EventTypes
{
public:
    BEGINE_EVENTS(CRUD_EventTypes)
    EVENT_DECL(evEntityEnteringMap      ,0)
    EVENT_DECL(evMapInstanceConnected   ,1)
    EVENT_DECL(evShortcutsRequest       ,2)
    EVENT_DECL(evShortcuts              ,3)
    EVENT_DECL(evSceneRequest           ,4)
    EVENT_DECL(evScene                  ,5)
    EVENT_DECL(evEntitiesRequest        ,6)
    EVENT_DECL(evEntitites              ,7)
    EVENT_DECL(evInputState             ,8)
    EVENT_DECL(evClientQuit             ,9)
    EVENT_DECL(evForceLogout            ,10)
    EVENT_DECL(evChatDividerMoved       ,36)
// client commands
    EVENT_DECL(evConsoleCommand         ,100)
    EVENT_DECL(evMiniMapState           ,101)
    EVENT_DECL(evClientResumedRendering ,104)
    EVENT_DECL(evCookieRequest          ,106)
    EVENT_DECL(evEnterDoor              ,109)
    EVENT_DECL(evSetDestination         ,111)
    EVENT_DECL(evWindowState            ,114)
    EVENT_DECL(evInspirationDockMode    ,117)
    EVENT_DECL(evChatMessage            ,120)
    EVENT_DECL(evAbortQueuedPower       ,133) // this might actually mean something different
    EVENT_DECL(evChangeStance           ,136)
    EVENT_DECL(evPlaqueVisited          ,139)
    EVENT_DECL(evCombineRequest         ,140)
    EVENT_DECL(evEntityInfoRequest      ,156)
    EVENT_DECL(evLocationVisited        ,162)
    EVENT_DECL(evClientSettings         ,165)
    EVENT_DECL(evDescriptionAndBattleCry,167)
    END_EVENTS(500)
};
//////////////////////////////////////////////////////////////////////////
// Client -> Server
#include "Events/NewEntity.h"

class MapUnknownRequest final : public MapLinkEvent
{
public:
    MapUnknownRequest():MapLinkEvent(MapEventTypes::evUnknownEvent)
    {
    }
    void serializeto(BitStream &) const override
    {
    }
    void serializefrom(BitStream &) override
    {
    }
};
class ShortcutsRequest final : public MapLinkEvent
{
public:
    ShortcutsRequest():MapLinkEvent(MapEventTypes::evShortcutsRequest)
    {}
    void serializeto(BitStream &bs) const override
    {
        bs.StorePackedBits(1,4); // opcode
    }
    void serializefrom(BitStream &) override
    {
    }
};

class SceneRequest final : public MapLinkEvent
{
public:
    SceneRequest():MapLinkEvent(MapEventTypes::evSceneRequest)
    {}
    void serializeto(BitStream &bs) const override
    {
        bs.StorePackedBits(1,3); // opcode
    }
    void serializefrom(BitStream &) override
    {
    }
};

class EntitiesRequest final : public MapLinkEvent
{
public:
    EntitiesRequest():MapLinkEvent(MapEventTypes::evEntitiesRequest)
    {}
    void serializeto(BitStream &bs) const override
    {
        bs.StorePackedBits(1,5); // opcode
    }
    void serializefrom(BitStream &) override
    {
    }
};
class ClientQuit final : public MapLinkEvent
{
public:
    int abort_disconnect = 0;
    ClientQuit():MapLinkEvent(MapEventTypes::evClientQuit)
    {}
    void serializeto(BitStream &bs) const override
    {
        bs.StorePackedBits(1,7); // opcode
        bs.StorePackedBits(1, abort_disconnect);
    }
    void serializefrom(BitStream &bs) override
    {
        abort_disconnect = bs.GetPackedBits(1);
        // TODO: try to differentiate between quit/logout ?
    }

};
class ForcedLogout final : public MapLinkEvent
{
public:
    QString reason;
    ForcedLogout(const QString &_reason) :MapLinkEvent(MapEventTypes::evForceLogout),reason(_reason)
    {}
    void serializeto(BitStream &bs) const override
    {
        bs.StorePackedBits(1,12); // opcode
        bs.StoreString(reason); // opcode
    }
    void serializefrom(BitStream &bs) override
    {
        bs.GetString(reason);
    }
};

class CookieRequest final : public MapLinkEvent
{
public:
    uint32_t cookie;
    uint32_t console;
    CookieRequest():MapLinkEvent(MapEventTypes::evCookieRequest)
    {}
    void serializeto(BitStream &bs) const override
    {
        bs.StorePackedBits(1,6); // opcode
    }
    void serializefrom(BitStream &bs) override
    {
        cookie = bs.GetPackedBits(1);
        console = bs.GetPackedBits(1);
    }
};
class ConsoleCommand final : public MapLinkEvent
{
public:
    QString contents;
    ConsoleCommand():MapLinkEvent(MapEventTypes::evConsoleCommand)
    {}
    void serializeto(BitStream &bs) const
    {
        bs.StorePackedBits(1,0); // opcode - Warning -> exactly the same as Connect
    }
    void serializefrom(BitStream &bs)
    {
        bs.GetString(contents);
    }
};
class ClientResumedRendering final : public MapLinkEvent
{
public:
    ClientResumedRendering():MapLinkEvent(MapEventTypes::evClientResumedRendering)
    {}
    void serializeto(BitStream &bs) const
    {
        bs.StorePackedBits(1,4);
    }
    void serializefrom(BitStream &bs)
    {
    }
};

class MiniMapState final : public MapLinkEvent
{

public:
    uint32_t tile_idx=0;
    MiniMapState():MapLinkEvent(MapEventTypes::evMiniMapState)
    {}
    void serializeto(BitStream &bs) const
    {
        bs.StorePackedBits(1,1); // opcode - Warning -> exactly the same as Connect
    }
    void serializefrom(BitStream &bs)
    {
        tile_idx=bs.GetPackedBits(8);
    }

};
class WindowState final : public MapLinkEvent
{
public:
    uint32_t window_idx;
    struct WindowS {
        uint32_t field_0;
        uint32_t field_4;
        uint32_t field_8;
        uint32_t field_C;
        uint32_t field_14;
        uint32_t field_18;
        uint32_t field_24;
        uint32_t color;
        uint32_t alpha;
    };
    WindowS wnd;
    WindowState():MapLinkEvent(MapEventTypes::evWindowState)
    {}
    void serializeto(BitStream &bs) const
    {
        bs.StorePackedBits(1,14); // opcode
    }
    void serializefrom(BitStream &bs)
    {
        window_idx = bs.GetPackedBits(1);

        wnd.field_0 = bs.GetPackedBits(1);
        wnd.field_4 = bs.GetPackedBits(1);
        uint32_t val = bs.GetPackedBits(1);
        if(val==4)
            wnd.field_18 = 2;
        wnd.field_24 = val;

        wnd.field_14 = bs.GetPackedBits(1);
        wnd.color = bs.GetPackedBits(1);
        wnd.alpha = bs.GetPackedBits(1);
        if(bs.GetBits(1)) {
            wnd.field_8 = bs.GetPackedBits(1);
            wnd.field_C = bs.GetPackedBits(1);
        }
    }
};

class CombineRequest final : public MapLinkEvent
{
public:
    struct PowerEntry
    {
        int powerset_array_index;
        int powerset_index;
        int index;
    };
    PowerEntry first_power;
    PowerEntry second_power;
    CombineRequest() : MapLinkEvent(MapEventTypes::evCombineRequest)
    {}
    void serializeto(BitStream &bs) const
    {
        bs.StorePackedBits(1,40); // opcode
        assert(false); // since we will not send CombineRequest to anyone :)
    }
    // now to make this useful ;)

    void getPowerForCombinde(BitStream &bs,PowerEntry &entry)
    {
        // first bit tells us if we have full/partial?? data
        // here we can do a small refactoring, because in both branches of if/else, the last set value is index.
        if(bs.GetBits(1))
        {
            entry.powerset_array_index = bs.GetPackedBits(1);
            entry.powerset_index = bs.GetPackedBits(1);
        }
        entry.index = bs.GetPackedBits(1);
    }
    void serializefrom(BitStream &bs)
    {
        getPowerForCombinde(bs,first_power);
        getPowerForCombinde(bs,second_power);
    }
};
#include "Events/InputState.h"
#include "Events/ChatMessage.h"
//////////////////////////////////////////////////////////////////////////
// Server -> Client events
class MapInstanceConnected final : public MapLinkEvent
{
public:
    MapInstanceConnected():MapLinkEvent(MapEventTypes::evMapInstanceConnected)
    {}
    MapInstanceConnected(EventProcessor *evsrc,uint32_t resp,const QString &err) :
        MapLinkEvent(MapEventTypes::evMapInstanceConnected,evsrc),
        m_resp(resp),
        m_fatal_error(err)
    {}

    void        serializeto(BitStream &bs) const
    {
        bs.StorePackedBits(1,5); //opcode
        bs.StorePackedBits(1,m_resp);
        if(m_resp)
            bs.StoreString(m_fatal_error);
    }
    void        serializefrom(BitStream &src)
    {
        m_resp = src.GetPackedBits(1);
        if(m_resp==0)
            src.GetString(m_fatal_error);
    }
    uint32_t    m_resp;
    QString m_fatal_error;

};


class InspirationDockMode final : public MapLinkEvent
{
public:
    uint32_t dock_mode=0;
    InspirationDockMode():MapLinkEvent(MapEventTypes::evInspirationDockMode)
    {}
    void serializeto(BitStream &bs) const
    {
        bs.StorePackedBits(1,17);
        bs.StoreBits(32,dock_mode);
    }
    void serializefrom(BitStream &bs)
    {
        dock_mode=bs.GetBits(32);
    }
};
class EnterDoor final : public MapLinkEvent
{
public:
    bool unspecified_location;
    glm::vec3 location;
    QString name;
    EnterDoor():MapLinkEvent(MapEventTypes::evEnterDoor)
    {}
    void serializeto(BitStream &bs) const
    {
        bs.StorePackedBits(1,9);
    }
    void serializefrom(BitStream &bs)
    {
        unspecified_location = bs.GetBits(1);
        if(!unspecified_location)
        {
            location.x = bs.GetFloat();
            location.y = bs.GetFloat();
            location.z = bs.GetFloat();
        }
        bs.GetString(name);
    }
};
class ChangeStance final : public MapLinkEvent
{
public:
    bool enter_stance;
    int powerset_index;
    int power_index;
    ChangeStance():MapLinkEvent(MapEventTypes::evChangeStance)
    {}
    void serializeto(BitStream &bs) const
    {
        bs.StorePackedBits(1,36);
    }
    void serializefrom(BitStream &bs)
    {
        enter_stance = bs.GetBits(1);
        if(!enter_stance)
            return;
        powerset_index=bs.GetPackedBits(4);
        power_index=bs.GetPackedBits(4);
    }

};
class SetDestination final : public MapLinkEvent
{
public:
    glm::vec3 destination;
    int point_index;
    SetDestination():MapLinkEvent(MapEventTypes::evSetDestination)
    {}
    void serializeto(BitStream &bs) const
    {
        bs.StorePackedBits(1,11);
        bs.StoreFloat(destination.x);
        bs.StoreFloat(destination.y);
        bs.StoreFloat(destination.z);
        bs.StorePackedBits(1,point_index);
    }
    void serializefrom(BitStream &bs)
    {
        destination.x = bs.GetFloat();
        destination.y = bs.GetFloat();
        destination.z = bs.GetFloat();
        point_index   = bs.GetPackedBits(1);
    }
};
class AbortQueuedPower final : public MapLinkEvent
{
public:
    AbortQueuedPower():MapLinkEvent(MapEventTypes::evAbortQueuedPower)
    {}
    void serializeto(BitStream &bs) const
    {
        bs.StorePackedBits(1,33);
    }
    void serializefrom(BitStream &bs)
    {
    }
};
class DescriptionAndBattleCry final : public MapLinkEvent
{
public:
    QString description;
    QString battlecry;
    DescriptionAndBattleCry():MapLinkEvent(MapEventTypes::evDescriptionAndBattleCry)
    {}
    void serializeto(BitStream &bs) const
    {
        bs.StorePackedBits(1,67);
    }
    void serializefrom(BitStream &bs)
    {
        bs.GetString(description);
        bs.GetString(battlecry);
    }
};
class EntityInfoRequest final : public MapLinkEvent
{
public:
    int entity_idx;
    EntityInfoRequest():MapLinkEvent(MapEventTypes::evEntityInfoRequest)
    {}
    void serializeto(BitStream &bs) const override
    {
        bs.StorePackedBits(1,56);
        bs.StorePackedBits(12,entity_idx);
    }
    void serializefrom(BitStream &bs) override
    {
        entity_idx = bs.GetPackedBits(12);
    }
};
#include "Events/ClientSettings.h"
#include "Events/ChatMessage.h"
#include "Events/ChatDividerMoved.h"
#include "Events/SceneEvent.h"
#include "Events/EntitiesResponse.h"
#include "Events/Shortcuts.h"
#include "Events/PlaqueVisited.h"
#include "Events/LocationVisited.h"
