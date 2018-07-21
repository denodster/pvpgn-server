/*
* Copyright (C) 2014  HarpyWar (harpywar@gmail.com)
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#ifndef INCLUDED_LOCALIZATION_TYPES
#define INCLUDED_LOCALIZATION_TYPES

namespace pvpgn
{
	namespace bnetd
	{

		typedef struct {
			t_gamelang gamelang;
			const char* name;
			std::vector<std::string> countries;
		} t_language;

		extern std::vector<t_language> languages;
	}
}

#endif

#ifndef JUST_NEED_TYPES
#ifndef INCLUDED_LOCALIZATION_PROTOS
#define INCLUDED_LOCALIZATION_PROTOS

#define JUST_NEED_TYPES
# include <string>
# include "connection.h"
#include <fmt/format.h>
#undef JUST_NEED_TYPES

namespace pvpgn
{

	namespace bnetd
	{
		extern int i18n_load(void);
		extern int i18n_reload(void);

		extern std::string i18n_filename(const char * filename, t_tag gamelang);
		extern t_language language_find_by_country(const char * code, bool &found);
		extern t_language language_find_by_tag(t_gamelang gamelang, bool &found);
		extern t_language language_get_by_country(const char * code);
		extern t_gamelang gamelang_get_by_country(const char * code);
		extern t_language language_get_by_tag(t_gamelang gamelang);

		extern t_gamelang conn_get_gamelang_localized(t_connection * c);

		extern int handle_language_command(t_connection * c, char const *text);
		extern char * i18n_convert(t_connection * c, char * buf);
		extern int tag_check_gamelang_real(t_tag gamelang);


		template <typename... Args>
		std::string _localize(t_connection * c, const char * func, fmt::string_view format_str, const Args& ... args)
		{
			const char *format = fmt;
			std::string output(fmt);
			t_gamelang lang;

			if (!c) {
				eventlog(eventlog_level_error, __FUNCTION__, "got bad connection");
				return format;
			}
			try
			{
				if (lang = conn_get_gamelang_localized(c))
					if (!(format = _find_string(fmt, lang)))
						format = fmt; // if not found use original

				output = fmt::format(format, args);

				char tmp[MAX_MESSAGE_LEN];
				std::snprintf(tmp, sizeof tmp, "%s", output.c_str());

				i18n_convert(c, tmp);
				output = tmp;
			}
			catch (const std::exception& e)
			{
				WARN2("Can't format translation string \"{}\" ({})", fmt, e.what());
			}

			return output;
		}

		#define localize(c, ...) _localize(c, __FUNCTION__, __VA_ARGS__)
	}

}

/*****/
#endif
#endif
