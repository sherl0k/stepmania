#ifndef GAMESTATE_H
#define GAMESTATE_H
/*
-----------------------------------------------------------------------------
 Class: GameState

 Desc: Holds game data that is not saved between sessions.

 Copyright (c) 2001-2002 by the person(s) listed below.  All rights reserved.
	Chris Danford
-----------------------------------------------------------------------------
*/

#include "GameConstantsAndTypes.h"
#include "PlayerOptions.h"
#include "SongOptions.h"
#include "Game.h"
#include "Style.h"
#include "Grade.h"
#include "StageStats.h"


class Song;
struct Notes;
class Course;
class GameDef;
class StyleDef;


class GameState
{
public:
	GameState();
	~GameState();
	void Reset();

	//
	// Main State Info
	//
	Game			m_CurGame;
	Style			m_CurStyle;
	bool			m_bPlayersCanJoin;	// true if it's not too late for a player to join - this only has an effect on the credits message
	bool			m_bSideIsJoined[NUM_PLAYERS];	// left side, right side
	int				m_iCoins;			// not "credits"
	PlayerNumber	m_MasterPlayerNumber;	// used in Styles where one player controls both sides
	int				GetNumSidesJoined()
	{ 
		int iNumSidesJoined = 0;
		for( int c=0; c<NUM_PLAYERS; c++ )
			if( m_bSideIsJoined[c] )
				iNumSidesJoined++;	// left side, and right side
		return iNumSidesJoined;
	};


	GameDef*	GetCurrentGameDef();
	const StyleDef*	GetCurrentStyleDef();

	bool IsPlayerEnabled( PlayerNumber pn );
	bool IsPlayerEnabled( int p ) { return IsPlayerEnabled( (PlayerNumber)p ); };	// for those too lasy to cast all those p's to a PlayerNumber

	CString			m_sLoadingMessage;	// used in loading screen
	CString			m_sPreferredGroup;	// "ALL MUSIC" denotes no preferred group
	Difficulty		m_PreferredDifficulty[NUM_PLAYERS];
	SongSortOrder	m_SongSortOrder;			// used by MusicWheel
	PlayMode		m_PlayMode;			// many screen display different info depending on this value
	bool			m_bEditing;			// NoteField does special stuff when this is true
	bool			m_bDemonstration;	// ScreenGameplay does special stuff when this is true
	int				m_iCurrentStageIndex;	// incremented on Eval screen

	int				GetStageIndex();
	bool			IsFinalStage();
	bool			IsExtraStage();
	bool			IsExtraStage2();
	CString			GetStageText();
	RageColor		GetStageColor();

	//
	// State Info used during gameplay
	//
	Song*		m_pCurSong;
	Notes*		m_pCurNotes[NUM_PLAYERS];
	Course*		m_pCurCourse;


	//
	// Music statistics:  Arcade: the current stage (one song).  Oni/Endles: a single song in a course
	//
	// Let a lot of classes access this info here so the don't have to keep their own copies.
	//
	float		m_fMusicSeconds;	// time into the current song
	float		m_fSongBeat;
	float		m_fCurBPS;
	bool		m_bFreeze;	// in the middle of a freeze
	bool		m_bPastHereWeGo;

	void ResetMusicStatistics();		// Call this when it's time to play a new song.  Clears the values above.
	
	//
	// Stage Statistics: 
	// Arcade:	for the current stage (one song).  
	// Nonstop/Oni/Endless:	 for current course (which usually contains multiple songs)
	//
	StageStats	m_CurStageStats;				// current stage (not necessarily passed if Extra Stage)
	vector<StageStats>	m_vPassedStageStats;	// Only useful in Arcade for final evaluation
												// A song is only inserted here if at least one player passed.
												// StageStats are added by the Evaluation screen
	void	GetFinalEvalStatsAndSongs( StageStats& statsOut, vector<Song*>& vSongsOut );	// shown on arcade final evaluation
	Grade	GetCurrentGrade( PlayerNumber pn );		// grade so far


	PlayerOptions	m_PlayerOptions[NUM_PLAYERS];    // The currently active options
	PlayerOptions   m_SelectedOptions[NUM_PLAYERS];  // Keep track of player-selected options for
													 // courses separately from the active options.
	SongOptions		m_SongOptions;


	bool HasEarnedExtraStage();


	// Filled in by ScreenNameEntry and used by ScreenRanking to flash the recent high scores
	NotesType			m_LastRankingNotesType;	// meaningless if a course was played
	RankingCategory		m_LastRankingCategory[NUM_PLAYERS];	// meaningless if a course was played
	Course*				m_pLastPlayedCourse;		// meaningless if arcade was played
	int					m_iLastRankingIndex[NUM_PLAYERS];		// -1 if no new high score
};


extern GameState*	GAMESTATE;	// global and accessable from anywhere in our program

#endif
