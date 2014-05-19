/*
 * GetOpt.h
 *
 *  Created on: Jan 12, 2014
 *      Author: kmahoney
 */

#ifndef GETOPT_H_
#define GETOPT_H_

#include <string>
#include <map>
#include <vector>

namespace zUtils
{

typedef struct Opt
{
    unsigned int flags;
    bool exists;
    std::string help;
    std::string arg;
} Opt;

class GetOpt
{
public:
    GetOpt( int argc, const char **argv );
    ~GetOpt();
    void SetDesc( const std::string &desc_ );
    void SetOpt( const std::string &opt_, const std::string &help_ );
    const std::string GetArg( const std::string &opt_ );
    void Parse();
    void Usage();
    bool Exists( const std::string &opt_ );

protected:

private:
    int argc;
    std::vector<std::string> argv;
    std::string progName;
    std::string progDesc;
    std::map<std::string, Opt> opt;

};

}

#endif /* GETOPT_H_ */
