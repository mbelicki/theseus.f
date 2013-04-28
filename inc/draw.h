#pragma once

#include "types.h"

void draw_free( const State * const state
              , SDL_Surface * const screen
              , const Assets * const assets
              );

void draw_splash( const State * const state
                , SDL_Surface * const screen
                , const Assets * const assets
                );

void draw_intro( const State * const state
               , SDL_Surface * const screen
               , const Assets * const assets
               );

void draw_dead( const State * const state
              , SDL_Surface * const screen
              , const Assets * const assets
              );

void draw_trade( const State * const state
               , SDL_Surface * const screen
               , const Assets * const assets
               );

void draw_boss( const State * const state
              , SDL_Surface * const screen
              , const Assets * const assets
              );

void draw_won( const State * const state
             , SDL_Surface * const screen
             , const Assets * const assets
             );

void draw_over( const State * const state
              , SDL_Surface * const screen
              , const Assets * const assets
              );
