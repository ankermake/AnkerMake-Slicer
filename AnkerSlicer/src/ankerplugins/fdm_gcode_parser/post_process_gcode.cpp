#include "post_process_gcode.h"
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
postProcessGcode::postProcessGcode()
{

}

void postProcessGcode::read_file(const std::string &file,callback_q callback)
{
    QString fileName = QString::fromStdString(file.c_str());
    QFile in(fileName);
    if (!in.open(QIODevice::ReadOnly | QIODevice::Text))
       return ;

    //in.setCodec(QTextCodec::codecForName("UTF-8"));
    m_parsing_file = true;
    char    lineBuf[4*1024]; //  Faster optimization add @2023-05-09 by ChunLian
    int64_t lineLen = 0;
    while (m_parsing_file && !in.atEnd())
    {
        //  QFile::readLine -> lineBuf include NewLine
        lineLen = in.readLine(lineBuf, sizeof(lineBuf)); //read one line at a time
        std::string_view line(lineBuf, static_cast<size_t>(lineLen) );
        callback(*this,line);
    }
    in.close();
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
