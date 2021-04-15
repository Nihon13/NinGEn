#ifndef NHNTXTCOL_H
#define NHNTXTCOL_H

#include <iostream>
#include <ctime>

namespace nhntxtcol {

    class TextColor
    {
        private:
            static bool withTimer;

            static void printfun(void)
            {
                std::cout << "\033[0m" << std::endl;
            }

            template<typename T, typename... Args>
            static void printfun(const T& value, const Args&... args)
            {
                std::cout << value;
                printfun(args...);
            }

            static std::string timefun()
            {
                char buf[12];
                std::time_t result = std::time(nullptr);
                std::tm* now = std::localtime(&result);
                strftime(buf, sizeof(buf), "[%H:%M:%S] ", now);

                return buf;
            }

        public:

            static void setTimerOn(void) { withTimer = true; }
            static void setTimerOff(void) { withTimer = false; }

            /////////////////////////////////////////////////////////////////////
            /*Bright black (gray) font color*/
            template<typename T, typename... Args>
            static void BCK(const T& t, const Args&... args)
            {
                std::cout << "\033[0;90m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bright red font color*/
            template<typename T, typename... Args>
            static void RED(const T& t, const Args&... args)
            {
                std::cout << "\033[0;91m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bright green font color*/
            template<typename T, typename... Args>
            static void GRN(const T& t, const Args&... args)
            {
                std::cout << "\033[0;92m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bright yellow font color*/
            template<typename T, typename... Args>
            static void YLW(const T& t, const Args&... args)
            {
                std::cout << "\033[0;93m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bright blue font color*/
            template<typename T, typename... Args>
            static void BLU(const T& t, const Args&... args)
            {
                std::cout << "\033[0;94m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bright pink/magenta/purple font color*/
            template<typename T, typename... Args>
            static void PNK(const T& t, const Args&... args)
            {
                std::cout << "\033[0;95m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bright cyan font color*/
            template<typename T, typename... Args>
            static void CYA(const T& t, const Args&... args)
            {
                std::cout << "\033[0;96m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bright white font color*/
            template<typename T, typename... Args>
            static void WHT(const T& t, const Args&... args)
            {
                std::cout << "\033[0;97m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////
            /*Bold bright black (gray) font color*/
            template<typename T, typename... Args>
            static void BBCK(const T& t, const Args&... args)
            {
                std::cout << "\033[1;90m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bold bright red font color*/
            template<typename T, typename... Args>
            static void BRED(const T& t, const Args&... args)
            {
                std::cout << "\033[1;91m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bold bright green font color*/
            template<typename T, typename... Args>
            static void BGRN(const T& t, const Args&... args)
            {
                std::cout << "\033[1;92m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bold bright yellow font color*/
            template<typename T, typename... Args>
            static void BYLW(const T& t, const Args&... args)
            {
                std::cout << "\033[1;93m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bold bright blue font color*/
            template<typename T, typename... Args>
            static void BBLU(const T& t, const Args&... args)
            {
                std::cout << "\033[1;94m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bold bright pink/magenta/purple font color*/
            template<typename T, typename... Args>
            static void BPNK(const T& t, const Args&... args)
            {
                std::cout << "\033[1;95m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bold bright cyan font color*/
            template<typename T, typename... Args>
            static void BCYA(const T& t, const Args&... args)
            {
                std::cout << "\033[1;96m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bold bright white font color*/
            template<typename T, typename... Args>
            static void BWHT(const T& t, const Args&... args)
            {
                std::cout << "\033[1;97m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////
            /*Bright black (gray) font color with underline*/
            template<typename T, typename... Args>
            static void UBCK(const T& t, const Args&... args)
            {
                std::cout << "\033[4;90m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bright red font color with underline*/
            template<typename T, typename... Args>
            static void URED(const T& t, const Args&... args)
            {
                std::cout << "\033[4;91m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bright green font color with underline*/
            template<typename T, typename... Args>
            static void UGRN(const T& t, const Args&... args)
            {
                std::cout << "\033[4;92m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bright yellow font color with underline*/
            template<typename T, typename... Args>
            static void UYLW(const T& t, const Args&... args)
            {
                std::cout << "\033[4;93m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bright blue font color with underline*/
            template<typename T, typename... Args>
            static void UBLU(const T& t, const Args&... args)
            {
                std::cout << "\033[4;94m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bright pink/magenta/purple font color with underline*/
            template<typename T, typename... Args>
            static void UPNK(const T& t, const Args&... args)
            {
                std::cout << "\033[4;95m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bright cyan font color with underline*/
            template<typename T, typename... Args>
            static void UCYA(const T& t, const Args&... args)
            {
                std::cout << "\033[4;96m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bright white font color with underline*/
            template<typename T, typename... Args>
            static void UWHT(const T& t, const Args&... args)
            {
                std::cout << "\033[4;97m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////
            /*Bright black (gray) font color with background*/
            template<typename T, typename... Args>
            static void GBCK(const T& t, const Args&... args)
            {
                std::cout << "\033[7;90m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bright red font color with background*/
            template<typename T, typename... Args>
            static void GRED(const T& t, const Args&... args)
            {
                std::cout << "\033[7;91m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bright green font color with background*/
            template<typename T, typename... Args>
            static void GGRN(const T& t, const Args&... args)
            {
                std::cout << "\033[7;92m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bright yellow font color with background*/
            template<typename T, typename... Args>
            static void GYLW(const T& t, const Args&... args)
            {
                std::cout << "\033[7;93m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bright blue font color with background*/
            template<typename T, typename... Args>
            static void GBLU(const T& t, const Args&... args)
            {
                std::cout << "\033[7;94m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bright pink/magenta/purple font color with background*/
            template<typename T, typename... Args>
            static void GPNK(const T& t, const Args&... args)
            {
                std::cout << "\033[7;95m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bright cyan font color with background*/
            template<typename T, typename... Args>
            static void GCYA(const T& t, const Args&... args)
            {
                std::cout << "\033[7;96m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
            /*Bright white font color with background*/
            template<typename T, typename... Args>
            static void GWHT(const T& t, const Args&... args)
            {
                std::cout << "\033[7;97m";
                if (withTimer)
                    printfun(timefun(), t, args...);
                else
                    printfun(t, args...);
            }
            /////////////////////////////////////////////////////////////////////
    };

}

#endif