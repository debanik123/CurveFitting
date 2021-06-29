


/*
* Curve Fitting
* See COPYRIGHT file at the top of the source tree.
*
* This product includes software developed by the
* STARGUE Project (http://www.stargue.org/).
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the STARGUE License Statement and
* the GNU General Public License along with this program. If not,
* see <http://www.lsstcorp.org/LegalNotices/>.
*/

/**
 * @file CommonCurve.h
 *
 * @brief common used functions
 *
 * @author Adama Zouma
 * 
 * @Contact: stargue49@gmail.com
 *
 */

#pragma once
#ifndef COMMON_CURVE_H
#define COMMON_CURVE_H

#include <iomanip> //std::left std::setw std::setfill
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>


namespace za
{
/* ============================================================================
 * Global Variables
 * ============================================================================
 */

extern std::string dataFile, parameter, path, pathRealData, pathParameter ;
extern struct stat info;

/* ============================================================================
 * Function Declaration
 * ============================================================================
 */

/**
 * \brief rint tabular     
 *
 * \details 
 *
 * \param t, type is T
 * \param width, type is int
 * \param o, type is O
 *
 * \return type is void
 * 
 * \see 
 * \attention
 */    
template<typename T, typename O> void printElement(T t, const int& width, O &o)
{
    const char separator    = ' ';
    o << std::left << std::setw(width) << std::setfill(separator) << t;
}

/**
 * \brief Current time    
 *
 * \details 
 *
 * \param empty, type is void
 *
 * \return type is string
 * 
 * \see 
 * \attention
 */   
std::string getCurrentTime()
{
    std::time_t now = std::time(NULL);
    std::tm * ptm = std::localtime(&now);
    char buffer[32];
    // Format: Mo, 15.06.2009 20:20:00
       std::strftime(buffer, 32, "%d_%m_%Y_%H_%M_%S_", ptm); 
    return std::string(buffer);
}

}

#endif /* COMMON_CURVE_H */
