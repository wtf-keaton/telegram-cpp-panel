#pragma once
#include <cstdint>

#include <format>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>

#define NOGDI // don't define stupid ERROR macro, kthx
#include <Windows.h>

// Credits: https://pastebin.com/rZrhPnWL
namespace util::log
{
    namespace detail
    {
        //
        // Constants
        //
        constexpr std::size_t kMaxLevelNameSize = 8;
        constexpr std::size_t kIndentationSize = 8; // in spaces

        //
        // A mutex that would be used to make sure that we are logging one line at the time
        //
        inline std::mutex _mtx = {};

        //
        // Just a set of colors for the verbose prefix in console
        //
        namespace colors
        {
            // clang-format off
            struct col_t { std::uint8_t fg, bg; };
            // clang-format on

            //
            // @credits: https://stackoverflow.com/a/54062826
            //
            inline constinit col_t NC = { 0, 0 };

            inline constinit col_t BLACK = { 30, 40 };
            inline constinit col_t RED = { 31, 41 };
            inline constinit col_t GREEN = { 32, 42 };
            inline constinit col_t YELLOW = { 33, 43 };
            inline constinit col_t BLUE = { 34, 44 };
            inline constinit col_t MAGNETA = { 35, 45 };
            inline constinit col_t CYAN = { 36, 46 };
            inline constinit col_t WHITE = { 37, 47 };

            inline constinit col_t BRIGHT_BLACK = { 90, 100 };
            inline constinit col_t BRIGHT_RED = { 91, 101 };
            inline constinit col_t BRIGHT_GREEN = { 92, 102 };
            inline constinit col_t BRIGHT_YELLOW = { 93, 103 };
            inline constinit col_t BRIGHT_BLUE = { 94, 104 };
            inline constinit col_t BRIGHT_MAGNETA = { 95, 105 };
            inline constinit col_t BRIGHT_CYAN = { 96, 106 };
            inline constinit col_t BRIGHT_WHITE = { 97, 107 };
        } // namespace colors

        //
        // Util function to set console colors
        // @todo: @es3n1n: maybe add font_style too?
        //
        inline void apply_style( std::uint8_t fg, std::uint8_t bg, const std::function<void( )> cb )
        {
            static std::once_flag fl;

            //
            // In order to use ANSI escape sequences we should make sure that we've activated virtual
            // terminal before using them
            //
            std::call_once( fl, [ ]( ) -> void
            {
                const HANDLE console_handle = GetStdHandle( STD_OUTPUT_HANDLE );

                if ( !console_handle )
                    throw std::runtime_error( "Unable to obtain console handle!" );

                //
                // Obtaining current console mode
                //
                DWORD console_flags = 0;
                GetConsoleMode( console_handle, &console_flags );

                //
                // Activating virtual terminal
                //
                console_flags |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
                if ( !SetConsoleMode( console_handle, console_flags ) )
                {
                    console_flags &= ~DISABLE_NEWLINE_AUTO_RETURN;

                    //
                    // Try again without DISABLE_NEWLINE_AUTO_RETURN
                    //
                    if ( !SetConsoleMode( console_handle, console_flags ) )
                        throw std::runtime_error( "Unable to activate virtual terminal :(" );
                }
            } );

            //
            // Applying style
            //
            const auto apply = [ ]( const std::uint8_t fg, const std::uint8_t bg ) -> void
            {
                std::cout << std::format( "\033[{};{}m", fg, bg );
            };
            apply( fg, bg );

            //
            // Invoke callback
            //
            cb( );

            //
            // Reset style
            //
            apply( colors::NC.fg, colors::NC.bg );
        }

        //
        // The main method that would be used to log lines
        //
        inline void log_line( const std::uint8_t indentation, const std::string_view level_name, const std::uint8_t color_fg, const std::uint8_t color_bg,
                              const std::string& msg )
        {
            std::lock_guard<decltype( _mtx )> _lock( _mtx );

            const auto indent = []( std::uint8_t ind ) [[msvc::forceinline]] -> void
            {
                if ( ind <= 0 )
                    return;

                apply_style( colors::BRIGHT_WHITE.fg, 0, [ = ]( ) -> void
                {
                    for ( std::size_t i = 0; i < ind; i++ )
                    {
                        std::cout << '|';

                        for ( std::size_t j = 0; j < kIndentationSize; j++ )
                            std::cout << ' ';
                    }
                } );
            };

            //
            // Iterating lines
            //
            std::string line;
            std::istringstream stream( msg );

            while ( std::getline( stream, line ) )
            {
                indent( indentation );

                std::cout << "[";
                apply_style( color_fg, color_bg, [ = ]( ) -> void { std::cout << std::format( "{:^{}}", level_name, kMaxLevelNameSize ); } );
                std::cout << "] " << line << "\n";
            }
        }
    } // namespace detail

#define MAKE_LOGGER_METHOD(fn_name, prefix, color_fg, color_bg)                                     \
    template <std::uint8_t IND = 0, typename... Args>                                               \
    inline void fn_name(const std::string_view fmt, Args... args) {                                 \
        detail::log_line(IND, prefix, detail::colors::##color_fg.fg, detail::colors::##color_bg.bg, \
                         std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...)));    \
    }

    MAKE_LOGGER_METHOD( debug, "debug", BRIGHT_WHITE, BLACK );
    MAKE_LOGGER_METHOD( info, "info", BRIGHT_GREEN, BLACK );
    MAKE_LOGGER_METHOD( warn, "warn", BRIGHT_YELLOW, BLACK );
    MAKE_LOGGER_METHOD( error, "error", BRIGHT_MAGNETA, BLACK );
    MAKE_LOGGER_METHOD( critical, "critical", BRIGHT_WHITE, MAGNETA );

    MAKE_LOGGER_METHOD( msg, "msg", BRIGHT_WHITE, BLACK );
    MAKE_LOGGER_METHOD( todo, "todo", BRIGHT_YELLOW, BLACK );
    MAKE_LOGGER_METHOD( fixme, "fixme", BRIGHT_YELLOW, BLACK );

#undef MAKE_LOGGER_METHOD
} // namespace util::log