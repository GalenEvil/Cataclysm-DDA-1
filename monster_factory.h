#ifndef MONSTER_FACTORY_H
#define MONSTER_FACTORY_H

#include "json.h"       // JsonObject|JsonArray loading
#include "translations.h"
#include "rng.h"

#include "monattack.h"  // Monster Special Attack Functions
#include "mondeath.h"   // Monster Death Functions
#include "enums.h"      // phase_id enum
#include "mtype.h"      // m_size, m_flag, monster_trigger enums

#include "color.h"

#include <string>
#include <set>
#include <map>

typedef void (mdeath::*MonDeathFunction)(game*, monster*);
typedef void (mattack::*MonAttackFunction)(game*, monster*);
typedef std::string Mon_Tag;

#define GetMon(x) monster_factory::factory().get_mon(x)

class monster_factory
{
    public:
        static monster_factory &factory()
        {
            static monster_factory _factory;

            return _factory;
        }

        static std::map<Mon_Tag, mtype*>::iterator get_mon_begin()
        {
            return factory().monsters.begin();
        }
        static std::map<Mon_Tag, mtype*>::iterator get_mon_end()
        {
            return factory().monsters.end();
        }
        static mtype *get_random_mon()
        {
            static std::vector<std::string> valid_random_types = factory().rnd_types();

            return factory().get_mon(valid_random_types[rng(0, valid_random_types.size())]);
        }


        // Load a monster definition "type":"monster"
        void load_monster(JsonObject &jo);
        // Load a species definition "type":"species"
        void load_species(JsonObject &jo);

        // Accessors
        mtype *get_mon(Mon_Tag tag);

    protected:
        std::map<Mon_Tag, mtype*> monsters;
        std::map<std::string, species_type*> species;
        std::set<std::string> categories;
    private:
        /** Default Constructor */
        monster_factory();
        std::vector<std::string> rnd_types()
        {
            std::vector<std::string> ret;
            for (std::map<std::string, mtype*>::iterator it = monsters.begin(); it != monsters.end(); ++it)
            {
                if (it->first != "mon_null" && it->first != "mon_generator")
                {
                    ret.push_back(it->first);
                }
            }

            return ret;
        }

        void finalize_monsters();

        // Data Initializers
        void init(); // from which all other general initializers are called
        void init_death_functions();
        void init_specattack_functions();
        void init_sizes();
        void init_flags();
        void init_triggers();
        void init_phases();

        // Data Loaders
        void set_mon_information(JsonObject &jo, mtype *mon); // id, name, description, phase, material, species, size
        void set_mon_triggers(JsonObject &jo, mtype *mon); // anger, placate, fear triggers
        void set_mon_categories(JsonObject &jo, mtype *mon); // categories
        void set_mon_data(JsonObject &jo, mtype *mon); // numerical information
        void set_mon_functions(JsonObject &jo, mtype *mon); // death and special attack functions
        void set_mon_flags(JsonObject &jo, mtype *mon); // m_flags

        void set_species_triggers(JsonObject &jo, species_type &type); // species default anger, placate, fear triggers
        void set_species_flags(JsonObject &jo, species_type &type); // categorical informations

        // Information manipulation Woo!
        void get_tags(JsonObject &jo, std::string membername, std::set<std::string> &data);

        // Data Accessors
        // Tag->Enum conversion

        template <typename T>  T convert_tag(std::string tag, std::map<Mon_Tag, T> tag_map, T fallback);

        // probably no longer needed?
        phase_id convert_tag(std::string tag, phase_id fallback);
        m_size convert_tag(std::string tag, m_size fallback);
        m_flag convert_tag(std::string tag, m_flag fallback);
        monster_trigger convert_tag(std::string tag, monster_trigger fallback);

        // Data
            // death function and special attack function string->function map
        std::map<Mon_Tag, MonDeathFunction> death_functions;
        std::map<Mon_Tag, MonAttackFunction> special_attack_functions;
            // size/flag/trigger/phase string->enumeration map
        std::map<Mon_Tag, m_size> size_tags;
        std::map<Mon_Tag, m_flag> flag_tags;
        std::map<Mon_Tag, monster_trigger> trigger_tags;
        std::map<Mon_Tag, phase_id> phase_tags;
            // monster storage

        // Defaults
        mtype *missing_monster_type;
        species_type *missing_species_type;
};

extern monster_factory *monster_controller;

#endif // MONSTER_FACTORY_H
