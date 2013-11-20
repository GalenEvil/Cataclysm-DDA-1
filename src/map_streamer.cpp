#include "map_streamer.h"
#include "debug.h"

map_streamer::map_streamer()
{
    //ctor
    streamable_submaps.clear();
    message_buffer.str("");
}

map_streamer::~map_streamer()
{
    //dtor
    dump_and_clear();
}

void map_streamer::dump_stream()
{
    if (message_buffer.str().size() > 0){
        DebugLog() << std::string(20, '*') << "\n";
        DebugLog() << streamable_submaps.size() << "\n";
    }
    DebugLog() << message_buffer.str();
    message_buffer.str("");
}

void map_streamer::dump_and_clear()
{
    dump_stream();
    streamable_submaps.clear();
}

void map_streamer::add_submap_to_stream(tripoint t)
{
    streamable_submaps.insert(t);
    message_buffer << "Submap definition added to streamable_submaps set at: <"<<t.x<<", "<<t.y<<", "<<t.z<<">\n";
}
