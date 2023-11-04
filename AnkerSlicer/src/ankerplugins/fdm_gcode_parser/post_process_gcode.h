#ifndef POSTPROCESSGCODE_H
#define POSTPROCESSGCODE_H
#include<QString>
#include <cstdlib>
#include <functional>
#include <string>
#include <string_view>
#include <cstring>
#include <functional>
class thumbnailInfo
{
     unsigned int w;
     unsigned int h;
     unsigned int file_size;
     char* data;
public:
    thumbnailInfo();

};

class postProcessGcode
{

public:
    class GCodeLine {
    public:
        GCodeLine() { reset(); }
        void reset() { m_raw.clear(); }

        const std::string&      raw() const { return m_raw; }
        const std::string_view  cmd() const {
            const char *cmd = postProcessGcode::skip_whitespaces(m_raw.c_str());
            return std::string_view(cmd, postProcessGcode::skip_word(cmd) - cmd);
        }
        const std::string_view  comment() const
            { size_t pos = m_raw.find(';'); return (pos == std::string::npos) ? std::string_view() : std::string_view(m_raw).substr(pos + 1); }


        bool cmd_is(const char *cmd_test) const {
            const char *cmd = postProcessGcode::skip_whitespaces(m_raw.c_str());
            size_t len = strlen(cmd_test);
            return strncmp(cmd, cmd_test, len) == 0 && postProcessGcode::is_end_of_word(cmd[len]);
        }
    private:
        std::string      m_raw;
        friend class postProcessGcode;
    };

    postProcessGcode();
    typedef std::function<void(postProcessGcode&, const GCodeLine&)> callback_t;
    typedef std::function<void(postProcessGcode&, const std::string_view&)> callback_q;
    template<typename Callback>
    const char* parse_line(const char *ptr, GCodeLine &gline, Callback &callback)
    {
        std::pair<const char*, const char*> cmd;
        const char *end = parse_line_internal(ptr, gline, cmd);
        callback(*this, gline);
        return end;
    }

    template<typename Callback>
    void parse_line(const std::string &line, Callback callback)
        { GCodeLine gline; this->parse_line(line.c_str(), gline, callback); }

    void read_file(const std::string &file,callback_q callback);
private:
    const char* parse_line_internal(const char *ptr, GCodeLine &gline, std::pair<const char*, const char*> &command);
    static bool         is_whitespace(char c)           { return c == ' ' || c == '\t'; }
    static bool         is_end_of_line(char c)          { return c == '\r' || c == '\n' || c == 0; }
    static bool         is_end_of_gcode_line(char c)    { return c == ';' || is_end_of_line(c); }
    static bool         is_end_of_word(char c)          { return is_whitespace(c) || is_end_of_gcode_line(c); }
    static const char*  skip_whitespaces(const char *c) {
        for (; is_whitespace(*c); ++ c)
            ; // silence -Wempty-body
        return c;
    }
    static const char*  skip_word(const char *c) {
        for (; ! is_end_of_word(*c); ++ c)
            ; // silence -Wempty-body
        return c;
    }

    bool        m_parsing_file{ false };

};

#endif // POSTPROCESSGCODE_H
