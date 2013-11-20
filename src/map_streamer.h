#ifndef MAP_STREAMER_H
#define MAP_STREAMER_H

#include "enums.h"
#include <set>
#include <sstream>

class map_streamer
{
    public:
        static map_streamer &MStream(){
            static map_streamer stream;

            return stream;
        }
        /** Default constructor */
        /** Default destructor */
        virtual ~map_streamer();

        void add_submap_to_stream(tripoint t);
        void dump_stream();
        void dump_and_clear();

        std::set<tripoint> streamable_submaps;
        std::stringstream message_buffer;
    protected:
    private:
        map_streamer();
};

#endif // MAP_STREAMER_H
