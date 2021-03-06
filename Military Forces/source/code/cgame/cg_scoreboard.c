/*
 * $Id: cg_scoreboard.c,v 1.6 2006-01-29 14:03:40 thebjoern Exp $
*/

// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_scoreboard -- draw the scoreboard on top of the game screen
#include "cg_local.h"


#define	SCOREBOARD_X		(0)

#define SB_HEADER			86
#define SB_TOP				(SB_HEADER+32)

// Where the status bar starts, so we don't overwrite it
#define SB_STATUSBAR		420

#define SB_NORMAL_HEIGHT	40
#define SB_INTER_HEIGHT		16 // interleaved height

#define SB_MAXCLIENTS_NORMAL  ((SB_STATUSBAR - SB_TOP) / SB_NORMAL_HEIGHT)
#define SB_MAXCLIENTS_INTER   ((SB_STATUSBAR - SB_TOP) / SB_INTER_HEIGHT - 1)

// Used when interleaved



#define SB_LEFT_BOTICON_X	(SCOREBOARD_X+0)
#define SB_LEFT_HEAD_X		(SCOREBOARD_X+32)
#define SB_RIGHT_BOTICON_X	(SCOREBOARD_X+64)
#define SB_RIGHT_HEAD_X		(SCOREBOARD_X+96)
// Normal
#define SB_BOTICON_X		(SCOREBOARD_X+32)
#define SB_HEAD_X			(SCOREBOARD_X+64)

#define SB_SCORELINE_X		112

#define SB_RATING_WIDTH	    (6 * BIGCHAR_WIDTH) // width 6
#define SB_SCORE_X			(SB_SCORELINE_X + BIGCHAR_WIDTH) // width 6
#define SB_RATING_X			(SB_SCORELINE_X + 6 * BIGCHAR_WIDTH) // width 6
#define SB_PING_X			(SB_SCORELINE_X + 12 * BIGCHAR_WIDTH + 8) // width 5
#define SB_TIME_X			(SB_SCORELINE_X + 17 * BIGCHAR_WIDTH + 8) // width 5
#define SB_NAME_X			(SB_SCORELINE_X + 22 * BIGCHAR_WIDTH) // width 15

// The New and improved score board
//
// In cases where the number of clients is high, the score board heads are interleaved
// here's the layout

//
//	0   32   80  112  144   240  320  400   <-- pixel position
//  bot head bot head score ping time name
//  
//  wins/losses are drawn on bot icon now

static bool localClient; // true if local client has been displayed


							 /*
=================
CG_DrawScoreboard
=================
*/
//static void CG_DrawClientScore( int y, score_t *score, float *color, float fade, bool largeFormat ) {
//	char	string[1024];
//	clientInfo_t	*ci;
//	int iconx;
//
//	if ( score->client < 0 || score->client >= cgs.maxclients ) {
//		Com_Printf( "Bad score->client: %i\n", score->client );
//		return;
//	}
//	
//	ci = &cgs.clientinfo[score->client];
//
//	iconx = SB_BOTICON_X + (SB_RATING_WIDTH / 2);
//
//	// draw the handicap or bot skill marker (unless player has flag)
//	if ( ci->objectives & OB_REDFLAG ) {
//		if( largeFormat ) {
//			CG_DrawFlagModel( iconx, y - ( 32 - BIGCHAR_HEIGHT ) / 2, 32, 32, ClientBase::TEAM_RED, false );
//		}
//		else {
//			CG_DrawFlagModel( iconx, y, 16, 16, ClientBase::TEAM_RED, false );
//		}
//	} else if ( ci->objectives & OB_BLUEFLAG ) {
//		if( largeFormat ) {
//			CG_DrawFlagModel( iconx, y - ( 32 - BIGCHAR_HEIGHT ) / 2, 32, 32, ClientBase::TEAM_BLUE, false );
//		}
//		else {
//			CG_DrawFlagModel( iconx, y, 16, 16, ClientBase::TEAM_BLUE, false );
//		}
//	} else {
////		if ( ci->botSkill > 0 && ci->botSkill <= 5 ) {
////		} else 
//		if ( ci->handicap < 100 ) {
//			Com_sprintf( string, sizeof( string ), "%i", ci->handicap );
//			if ( cgs.gametype == GT_TOURNAMENT )
//				CG_DrawSmallStringColor( iconx, y - SMALLCHAR_HEIGHT/2, string, color );
//			else
//				CG_DrawSmallStringColor( iconx, y, string, color );
//		}
//
//		// draw the wins / losses
//		if ( cgs.gametype == GT_TOURNAMENT ) {
//			Com_sprintf( string, sizeof( string ), "%i/%i", ci->wins, ci->losses );
//			if( ci->handicap < 100 && !ci->botSkill ) {
//				CG_DrawSmallStringColor( iconx, y + SMALLCHAR_HEIGHT/2, string, color );
//			}
//			else {
//				CG_DrawSmallStringColor( iconx, y, string, color );
//			}
//		}
//	}
//
//	// draw the score line
//	if ( score->ping == -1 ) {
//		Com_sprintf(string, sizeof(string),
//			" connecting    %s", ci->name);
//	} else if ( ci->team == ClientBase::TEAM_SPECTATOR ) {
//		Com_sprintf(string, sizeof(string),
//			" SPECT %3i %4i %s", score->ping, score->time, ci->name);
//	} else {
//		Com_sprintf(string, sizeof(string),
//			"%5i %4i %4i %s", score->score, score->ping, score->time, ci->name);
//	}
//
//	// highlight your position
//	if ( score->client == cg.snap->ps.clientNum ) {
//		float	hcolor[4];
//		int		rank;
//
//		localClient = true;
//
//		if ( cg.snap->ps.persistant[PERS_TEAM] == ClientBase::TEAM_SPECTATOR 
//			|| cgs.gametype >= GT_TEAM ) {
//			rank = -1;
//		} else {
//			rank = cg.snap->ps.persistant[PERS_RANK] & ~RANK_TIED_FLAG;
//		}
//		if ( rank == 0 ) {
//			hcolor[0] = 0;
//			hcolor[1] = 0;
//			hcolor[2] = 0.7f;
//		} else if ( rank == 1 ) {
//			hcolor[0] = 0.7f;
//			hcolor[1] = 0;
//			hcolor[2] = 0;
//		} else if ( rank == 2 ) {
//			hcolor[0] = 0.7f;
//			hcolor[1] = 0.7f;
//			hcolor[2] = 0;
//		} else {
//			hcolor[0] = 0.7f;
//			hcolor[1] = 0.7f;
//			hcolor[2] = 0.7f;
//		}
//
//		hcolor[3] = fade * 0.7;
//		CG_FillRect( SB_SCORELINE_X + BIGCHAR_WIDTH + (SB_RATING_WIDTH / 2), y, 
//			640 - SB_SCORELINE_X - BIGCHAR_WIDTH, BIGCHAR_HEIGHT+1, hcolor );
//	}
//
//	CG_DrawBigString( SB_SCORELINE_X + (SB_RATING_WIDTH / 2), y, string, fade );
//
//	// add the "ready" marker for intermission exiting
//	if ( cg.snap->ps.stats[ STAT_CLIENTS_READY ] & ( 1 << score->client ) ) {
//		CG_DrawBigStringColor( iconx, y, "READY", color );
//	}
//}

/*
=================
CG_TeamScoreboard
=================
*/
//static int CG_TeamScoreboard( int y, team_t team, float fade, int maxClients, int lineHeight ) {
//	int		i;
//	score_t	*score;
//	float	color[4];
//	int		count;
//	clientInfo_t	*ci;
//
//	color[0] = color[1] = color[2] = 1.0;
//	color[3] = fade;
//
//	count = 0;
//	for ( i = 0 ; i < cg.numScores && count < maxClients ; i++ ) {
//		score = &cg.scores[i];
//		ci = &cgs.clientinfo[ score->client ];
//
//		if ( team != ci->team ) {
//			continue;
//		}
//
//		CG_DrawClientScore( y + lineHeight * count, score, color, fade, lineHeight == SB_NORMAL_HEIGHT );
//
//		count++;
//	}
//
//	return count;
//}

//================================================================================

/*
================
CG_CenterGiantLine
================
*/
static void CG_CenterGiantLine( float y, const char *string ) {
	float		x;
	vec4_t		color;

	color[0] = 1;
	color[1] = 1;
	color[2] = 1;
	color[3] = 1;

	x = 0.5 * ( 640 - GIANT_WIDTH * CG_DrawStrlen( string ) );

	CG_DrawStringExt( x, y, string, color, true, true, GIANT_WIDTH, GIANT_HEIGHT, 0 );
}

/*
=================
CG_DrawTourneyScoreboard

Draw the oversize scoreboard for tournements
=================
*/
void CG_DrawOldTourneyScoreboard( void ) {
	const char		*s;
	vec4_t			color;
	int				min, tens, ones;
	clientInfo_t	*ci;
	int				y;
	int				i;

	// request more scores regularly
	if ( cg.scoresRequestTime + 2000 < cg.time ) {
		cg.scoresRequestTime = cg.time;
		CL_AddReliableCommand( "score" );
	}

	color[0] = 1;
	color[1] = 1;
	color[2] = 1;
	color[3] = 1;

	// draw the dialog background
	color[0] = color[1] = color[2] = 0;
	color[3] = 1;
	CG_FillRect( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, color );

	// print the mesage of the day
	s = CG_ConfigString( CS_MOTD );
	if ( !s[0] ) {
		s = "Scoreboard";
	}

	// print optional title
	CG_CenterGiantLine( 8, s );

	// print server time
	ones = cg.time / 1000;
	min = ones / 60;
	ones %= 60;
	tens = ones / 10;
	ones %= 10;
	s = va("%i:%i%i", min, tens, ones );

	CG_CenterGiantLine( 64, s );


	// print the two scores

	y = 160;
	if ( cgs.gametype >= GT_TEAM ) {
		//
		// teamplay scoreboard
		//
		CG_DrawStringExt( 8, y, "Red Team", color, true, true, GIANT_WIDTH, GIANT_HEIGHT, 0 );
		s = va("%i", cg.teamScores[0] );
		CG_DrawStringExt( 632 - GIANT_WIDTH * strlen(s), y, s, color, true, true, GIANT_WIDTH, GIANT_HEIGHT, 0 );
		
		y += 64;

		CG_DrawStringExt( 8, y, "Blue Team", color, true, true, GIANT_WIDTH, GIANT_HEIGHT, 0 );
		s = va("%i", cg.teamScores[1] );
		CG_DrawStringExt( 632 - GIANT_WIDTH * strlen(s), y, s, color, true, true, GIANT_WIDTH, GIANT_HEIGHT, 0 );
	} else {
		//
		// free for all scoreboard
		//
		for ( i = 0 ; i < MAX_CLIENTS ; i++ ) {
			ci = &cgs.clientinfo[i];
			if ( !ci->infoValid ) {
				continue;
			}
			if ( ci->team != ClientBase::TEAM_FREE ) {
				continue;
			}

			CG_DrawStringExt( 8, y, ci->name, color, true, true, GIANT_WIDTH, GIANT_HEIGHT, 0 );
			s = va("%i", ci->score );
			CG_DrawStringExt( 632 - GIANT_WIDTH * strlen(s), y, s, color, true, true, GIANT_WIDTH, GIANT_HEIGHT, 0 );
			y += 64;
		}
	}


}

