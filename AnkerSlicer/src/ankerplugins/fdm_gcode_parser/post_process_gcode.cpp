#include "post_process_gcode.h"
#include <QFile>
#include <QTextStream>
postProcessGcode::postProcessGcode()
{

}

void postProcessGcode::read_file(const std::string &file,callback_q callback)
{
    QString fileName = QString::fromStdString(file.c_str());
    QFile f(fileName);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
       return ;

    QTextStream in(&f);
    m_parsing_file = true;
    while (m_parsing_file && !in.atEnd())
    {
       QString line = in.readLine(); //read one line at a time
       //this->parse_line(line.toStdString(), callback);
        callback(*this,line);
    }
    f.close();

}


const char* postProcessGcode::parse_line_internal(const char *ptr, GCodeLine &gline, std::pair<const char*, const char*> &command)
{
    //PROFILE_FUNC();

    // command and args
    const char *c = ptr;
    {
        //PROFILE_BLOCK(command_and_args);
        // Skip the whitespaces.
        command.first = skip_whitespaces(c);
        // Skip the command.
        c = command.second = skip_word(command.first);
        // Up to the end of line or comment.
        while (! is_end_of_gcode_line(*c)) {
            // Skip whitespaces.
            c = skip_whitespaces(c);
            if (is_end_of_gcode_line(*c))
                break;
            // Check the name of the axis.
    }

    }
    // Skip the rest of the line.
    for (; ! is_end_of_line(*c); ++ c);

    // Copy the raw string including the comment, without the trailing newlines.
    if (c > ptr) {
        //PROFILE_BLOCK(copy_raw_string);
        gline.m_raw.assign(ptr, c);
    }

    // Skip the trailing newlines.
    if (*c == '\r')
        ++ c;
    if (*c == '\n')
        ++ c;

    return c;
}
