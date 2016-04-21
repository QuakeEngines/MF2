/*
 * $Id: cg_local.h,v 1.21 2007-06-17 17:11:12 minkis Exp $
*/

// Copyright (C) 1999-2000 Id Software, Inc.
//
#ifndef __CG_LOCAL_H__
#define __CG_LOCAL_H__

#include "../game/q_shared.h"
#include "../qcommon/qfiles.h"
#include "tr_types.h"
#include "../game/bg_public.h"
#include "cg_public.h"
#include "../ui/ui_shared.h"
#include "../renderer/tr_public.h"
#include "../vehiclelib/bg_datamanager.h"
#include "../vehiclelib/bg_vehicleinfo.h"
#include "../vehiclelib/bg_weaponinfo.h"

// used for 2D drawing using the shared UI code
//extern displayContextDef_t * DC;

// global client enumerations

typedef enum {
	EXPLODE_VEHICLE_DIE,
	EXPLODE_VEHICLE_GIB,
	EXPLODE_BUILDING
} explosion_t;

typedef enum {
	LOW_QUALITY,
	MEDIUM_QUALITY,
	HIGH_QUALITY
} quality_t;

// The entire cgame module is unloaded and reloaded on each level change,
// so there is NO persistant data between levels on the client side.
// If you absolutely need something stored, it can either be kept
// by the server in the server stored userinfos, or stashed in a cvar.

#define	MAX_RETICLES		2

#define	POWERUP_BLINKS		5

#define	POWERUP_BLINK_TIME	1000
#define	FADE_TIME			200
#define	PULSE_TIME			200
#define	DAMAGE_DEFLECT_TIME	100
#define	DAMAGE_RETURN_TIME	400
#define DAMAGE_TIME			500
#define	LAND_DEFLECT_TIME	150
#define	LAND_RETURN_TIME	300
#define	STEP_TIME			200
#define	DUCK_TIME			100
#define	PAIN_TWITCH_TIME	200
#define	WEAPON_SELECT_TIME	1400
#define	ITEM_SCALEUP_TIME	1000
#define	ZOOM_TIME			150
#define	ITEM_BLOB_TIME		200
#define	MUZZLE_FLASH_TIME	20
#define	SINK_TIME			1000		// time for fragments to sink into ground before going away
#define	ATTACKER_HEAD_TIME	10000
#define	REWARD_TIME			3000

#define	PULSE_SCALE			1.5			// amount to scale up the icons when activating

#define	MAX_STEP_CHANGE		32

#define	MAX_VERTS_ON_POLY	10
#define	MAX_MARK_POLYS		256

#define STAT_MINUS			10	// num frame for '-' stats digit
#define STAT_COLON			11	// num frame for ':' stats digit
#define STAT_PERCENT		12	// num frame for '%' stats digit
#define STAT_POINT			13	// num frame for '.' stats digit

#define HUDNUM_WIDTH		8
#define HUDNUM_HEIGHT		8

#define	ICON_SIZE			48
#define	CHAR_WIDTH			32
#define	CHAR_HEIGHT			48
#define	TEXT_ICON_SPACE		4

#define	TEAMCHAT_WIDTH		80
#define TEAMCHAT_HEIGHT		8

// very large characters
#define	GIANT_WIDTH			32
#define	GIANT_HEIGHT		48

#define TEAM_OVERLAY_MAXNAME_WIDTH	12
#define TEAM_OVERLAY_MAXLOCATION_WIDTH	16

#define DEFAULT_REDTEAM_NAME		"Rebels"
#define DEFAULT_BLUETEAM_NAME		"Coalition"

typedef enum {
	IMPACTSOUND_DEFAULT,
	IMPACTSOUND_METAL,
	IMPACTSOUND_FLESH
} impactSound_t;

// radar
#define RD_GROUND_ENEMY			0
#define RD_GROUND_FRIEND		1
#define RD_GROUND_UNKNOWN		2
#define RD_BUILDING_ENEMY		3
#define RD_BUILDING_FRIEND		4
#define RD_BUILDING_UNKNOWN		5
#define RD_AIR_SAME_ENEMY		6
#define RD_AIR_SAME_FRIEND		7
#define RD_AIR_SAME_UNKNOWN		8
#define RD_AIR_HIGH_ENEMY		9
#define RD_AIR_HIGH_FRIEND		10
#define RD_AIR_HIGH_UNKNOWN		11
#define RD_AIR_LOW_ENEMY		12
#define RD_AIR_LOW_FRIEND		13
#define RD_AIR_LOW_UNKNOWN		14
#define RD_MAX_ICONS			15

#define MAX_RADAR_TARGETS		32

// MFD
typedef enum {
	MFD_OFF,
	MFD_RWR,
	MFD_STATUS,
	MFD_INVENTORY,
	MFD_CAMERA,
	MFD_INFO,
	MFD_MAX					
} MFD_Modes;

#define MFD_1					0
#define MFD_2					1
#define NUM_MFDS_IN_COCKPIT		2

// cameramode
typedef enum {
	CAMERA_TARGET,
	CAMERA_BACK,
	CAMERA_DOWN,
	CAMERA_BOMB,
	CAMERA_MAX		
} Camera_Modes;

#define MAX_CONSOLE_LINES 8

// custom console
struct strConsoleLine {
	char text[ 1024 ];	// TODO: possibly make this dynamic to save memory
	float life;
};

//=================================================

// player entities need to track more information
// than any other type of entity.

// note that not every player entity is a client entity,
// because corpses after respawn are outside the normal
// client numbering range

typedef struct {
	int				painTime;
	int				painDirection;	// flip from 0 to 1

} playerEntity_t;

//=================================================



// centity_t have a direct corespondence with gentity_t in the game, but
// only the entityState_t is directly communicated to the cgame
typedef struct centity_s {
	entityState_t	currentState;	// from cg.frame
	entityState_t	nextState;		// from cg.nextFrame, if available
	bool		interpolate;	// true if next is valid to interpolate to
	bool		currentValid;	// true if cg.frame holds this entity

	int				muzzleFlashTime;	// move to playerEntity?
	int				previousEvent;
	int				teleportFlag;

	int				trailTime;		// so missile trails can handle dropped initial packets
	int				dustTrailTime;
	int				miscTime;
	int				TimeSinceLastTrail;	// Time since last trail was drawn


	int				snapShotTime;	// last time this entity was found in a snapshot

	playerEntity_t	pe;

	int				errorTime;		// decay the error from this time
	vec3_t			errorOrigin;
	vec3_t			errorAngles;
	
	bool		extrapolated;	// false if origin / angles is an interpolation
	vec3_t			rawOrigin;
	vec3_t			rawAngles;

	vec3_t			beamEnd;
	vec3_t			lastDrawnTrailPos;		// Position of last drawn trail
	// exact interpolated position of entity on this frame
	vec3_t			lerpOrigin;
	vec3_t			lerpAngles;

	bool		destroyableStructure;

	int				gearAnim;
	int				gearAnimFrame;
	int				gearAnimStartTime;

	int				bayAnim;
	int				bayAnimFrame;
	int				bayAnimStartTime;

	int				muzzleFlashWeapon;
} centity_t;


//======================================================================

// local entities are created as a result of events or predicted actions,
// and live independantly from all server transmitted entities

typedef struct markPoly_s {
	struct markPoly_s	*prevMark, *nextMark;
	int			time;
	qhandle_t	markShader;
	bool	alphaFade;		// fade alpha instead of rgb
	float		color[4];
	poly_t		poly;
	polyVert_t	verts[MAX_VERTS_ON_POLY];
} markPoly_t;


typedef enum {
	LE_MARK,
	LE_EXPLOSION,
	LE_SPRITE_EXPLOSION,
	LE_FRAGMENT,
	LE_MOVE_SCALE_FADE,
	LE_FADE_RGB,
	LE_SCALE_FADE,
	LE_NUKE
} leType_t;

typedef enum {
	LEF_PUFF_DONT_SCALE  = 0x0001,			// do not scale size over time
	LEF_TUMBLE			 = 0x0002,			// tumble over time, used for ejecting shells
	LEF_NO_RADIUS_KILL	 = 0x0004,			// don't CG_FreeLocalEntity() just because the camera is 'inside' the radius of a sprite (e.g. smoke puffs)
	LEF_SOUND1			 = 0x0004,			// sound 1 for nuke
	LEF_SOUND2			 = 0x0008			// sound 2 for nuke
} leFlag_t;

typedef enum {
	LEMT_NONE,
	LEMT_BURN,
	LEMT_BLOOD
} leMarkType_t;			// fragment local entities can leave marks on walls

typedef enum {
	LEBS_NONE,
	LEBS_BRASS,
	LEBS_BLOOD
} leBounceSoundType_t;	// fragment local entities can make sounds on impacts

typedef struct localEntity_s {
	struct localEntity_s	*prev, *next;
	leType_t		leType;
	int				leFlags;

	int				startTime;
	int				endTime;
	int				fadeInTime;

	float			lifeRate;			// 1.0 / (endTime - startTime)

	trajectory_t	pos;
	trajectory_t	angles;

	float			bounceFactor;		// 0.0 = no bounce, 1.0 = perfect

	float			color[4];

	float			radius;

	float			light;
	vec3_t			lightColor;

	leMarkType_t		leMarkType;		// mark to leave on fragment impact
	leBounceSoundType_t	leBounceSoundType;

	refEntity_t		refEntity;		
} localEntity_t;

//======================================================================


typedef struct {
	int				client;
	int				score;
	int				ping;
	int				time;
	int				scoreFlags;
	int				powerUps;
	int				accuracy;
	int				impressiveCount;
	int				excellentCount;
	int				guantletCount;
	int				defendCount;
	int				assistCount;
	int				captures;
	int				deaths;
	int				perfect;
	int				team;
} score_t;

// each client has an associated clientInfo_t
// that contains media references necessary to present the
// client model and other color coded effects
// this is regenerated each time a client's configstring changes,
// usually as a result of a userinfo (name, model, etc) change
#define	MAX_CUSTOM_SOUNDS	32

typedef struct {
	bool		infoValid;

	char			name[MAX_QPATH];
	ClientBase::eTeam team;

	int				botSkill;		// 0 = not bot, 1-5 = bot

	vec3_t			color;

	int				score;			// updated by score servercmds
	int				location;		// location index for team mode
	int				health;			// you only get this info about your teammates
	int				curWeapon;

	int				handicap;
	int				wins, losses;	// in tourney mode

	int				teamTask;		// task in teamplay (offence/defence)
	int				teamLeader;		// true when this is a team leader

	int				objectives;		// so can display flag status

	int				invulnerabilityStartTime;
	int				invulnerabilityStopTime;

	char			redTeam[MAX_TEAMNAME];
	char			blueTeam[MAX_TEAMNAME];

	gender_t		gender;			// from model

	sfxHandle_t		sounds[MAX_CUSTOM_SOUNDS];

	// mfq3 specific
	int				vehicle;
	int				deaths;
	qhandle_t		parts[BP_MAX_PARTS];
	int				advancedControls;
} clientInfo_t;

// each WP_* weapon enum has an associated weaponInfo_t
// that contains media references necessary to present the
// weapon and its effects
typedef struct weaponInfo_s {
	bool		registered;

	float			flashDlight;
	vec3_t			flashDlightColor;
	sfxHandle_t		flashSound[4];		// fast firing weapons randomly choose

	qhandle_t		ammoModel;

	qhandle_t		missileModel;
	sfxHandle_t		missileSound;
	void			(*missileTrailFunc)( centity_t *, const struct weaponInfo_s *wi );
	float			missileDlight;
	vec3_t			missileDlightColor;
	int				missileRenderfx;

	float			trailRadius;
	float			wiTrailTime;

	sfxHandle_t		readySound;
	sfxHandle_t		firingSound;
} weaponInfo_t;


// each IT_* item has an associated itemInfo_t
// that constains media references necessary to present the
// item and its effects
typedef struct {
	bool		registered;
	qhandle_t		models[MAX_ITEM_MODELS];
	qhandle_t		icon;
} itemInfo_t;


typedef struct {
	int				itemNum;
} powerupInfo_t;

// reticle structure passed to the HUD code for rendering
typedef struct {
	int ox;	// real x & y
	int oy;
	int x;	// draw x & y
	int y;
	int w;	// width & height
	int h;
	qhandle_t shader;		// shader index (gfx)
	centity_t * pTarget;	// target entity pointer (can be NULL)
} reticle_t;

#define MAX_REWARDSTACK		10
#define MAX_SOUNDBUFFER		20

//======================================================================

// occurs, and they will have visible effects for #define STEP_TIME or whatever msec after

#define MAX_PREDICTED_EVENTS	16
 
typedef struct {
	int			clientFrame;		// incremented each frame

	int			clientNum;
	
	bool		demoPlayback;
	bool		levelShot;			// taking a level menu screenshot
	int			deferredPlayerLoading;
	int			intermissionStarted;	// don't play voice rewards, because game will end shortly

	// there are only one or two snapshot_t that are relevent at a time
	int			latestSnapshotNum;	// the number of snapshots the client system has received
	int			latestSnapshotTime;	// the time from latestSnapshotNum, so we don't need to read the snapshot yet

	snapshot_t	*snap;				// cg.snap->serverTime <= cg.time
	snapshot_t	*nextSnap;			// cg.nextSnap->serverTime > cg.time, or NULL
	snapshot_t	activeSnapshots[2];

	float		frameInterpolation;	// (float)( cg.time - cg.frame->serverTime ) / (cg.nextFrame->serverTime - cg.frame->serverTime)

	bool	thisFrameTeleport;
	bool	nextFrameTeleport;

	int			frametime;		// cg.time - cg.oldTime

	int			time;			// this is the time value that the client
								// is rendering at.
	int			oldTime;		// time at last frame, used for missile trails and prediction checking

	int			physicsTime;	// either cg.snap->time or cg.nextSnap->time

	int			timelimitWarnings;	// 5 min, 1 min, overtime
	int			fraglimitWarnings;

	bool	mapRestart;			// set on a map restart to set back the weapon

	bool	renderingThirdPerson;		// during deaths, chasecams, etc

	// prediction state
	bool	hyperspace;				// true if prediction has hit a trigger_teleport
	playerState_t	predictedPlayerState;
	centity_t		predictedPlayerEntity;
	bool	validPPS;				// clear until the first call to CG_PredictPlayerState
	int			predictedErrorTime;
	vec3_t		predictedError;

	int			eventSequence;
	int			predictableEvents[MAX_PREDICTED_EVENTS];

	// input state sent to server
	int			weaponSelect;

	// auto rotating items
	vec3_t		autoAngles;
	vec3_t		autoAxis[3];
	vec3_t		autoAnglesFast;
	vec3_t		autoAxisFast[3];

	// view rendering
	refdef_t	refdef;
	vec3_t		refdefViewAngles;		// will be converted to refdef.viewaxis

	// zoom key
	bool	zoomed;					// are we applying any zoom?
	bool	zoomLockOut;			// lock zoom until zoom button releases
	int			zoomAmount;				// x2, x4, x8 ...
	int			zoomTime;				// (unused)
	float		zoomSensitivity;		// (unused)

	// camera adjusting
	int			cameraAdjustEnum;		// camera adjustment identifier enum
	float		cameraAdjustAmount;		// camera adjustment amount
	int			cameraAdjustCount;

	// information screen text during loading
	char		infoScreenText[MAX_STRING_CHARS];

	// scoreboard
	int			scoresRequestTime;
	int			numScores;
	int			selectedScore;
	int			teamScores[2];
	score_t		scores[MAX_CLIENTS];
	bool	showScores;
	bool	scoreBoardShowing;
	int			scoreFadeTime;
	char		killerName[MAX_NAME_LENGTH];
	char			spectatorList[MAX_STRING_CHARS];		// list of names
	int				spectatorLen;												// length of list
	float			spectatorWidth;											// width in device units
	int				spectatorTime;											// next time to offset
	char		scoreboardMisc[MAX_STRING_CHARS];	// scoreboard extra info

	// centerprinting
	int			centerPrintTime;
	int			centerPrintCharWidth;
	int			centerPrintY;
	char		centerPrint[1024];
	int			centerPrintLines;

	// kill timers for carnage reward
	int			lastKillTime;

	// crosshair client ID
	int			crosshairClientNum;
	int			crosshairClientTime;

	// attacking player
	int			attackerTime;

	// reward medals
	int			rewardStack;
	int			rewardTime;
	int			rewardCount[MAX_REWARDSTACK];
	qhandle_t	rewardShader[MAX_REWARDSTACK];
	qhandle_t	rewardSound[MAX_REWARDSTACK];

	// sound buffer mainly for announcer sounds
	int			soundBufferIn;
	int			soundBufferOut;
	int			soundTime;
	qhandle_t	soundBuffer[MAX_SOUNDBUFFER];

	// warmup countdown
	int			warmup;
	int			warmupCount;

	//==========================

	int			itemPickup;
	int			itemPickupTime;
	int			itemPickupBlendTime;	// the pulse around the crosshair is timed seperately

	// blend blobs
	float		damageTime;
	float		damageX, damageY, damageValue;

	// view movement
	float		v_dmg_time;
	float		v_dmg_pitch;
	float		v_dmg_roll;

	vec3_t		kick_angles;	// weapon kicks
	vec3_t		kick_origin;

	// development tool
	refEntity_t		testModelEntity;
	char			testModelName[MAX_QPATH];
	bool		testGun;
	refEntity_t		testVehicleParts[BP_MAX_PARTS];
	char			testVehicleName[MAX_QPATH];
	long			testVehicleCat;
	refEntity_t		testBurner;
	refEntity_t		testBurner2;
	int				testBurnerNum;
	int				testNumBurners;
	refEntity_t		testVapor;
	int				testVaporNum;
	int				testVaporFrame;

	// MFQ3
	int				INFO;
	int				INFOTime;
	int				RADARTime;
	int				RADARRangeSetting;
	int				GPS;
	int				GPSTime;
	int				MFDTime;
	int				Mode_MFD[NUM_MFDS_IN_COCKPIT];
	int				toggleViewTime;
	centity_t		*radarEnts[MAX_RADAR_TARGETS];
	unsigned int	radarTargets;
	int				tracernum;
	int				HUDColor;
	int				HUDColorTime;
	int				MFDColor;
	int				MFDColorTime;
	refdef_t		HUDCamera;
	int				CameraMode;
	int				CameraModeTime;
	bool		drawingMFD;
	reticle_t		HUDReticle[ MAX_RETICLES ];
	int				reticleIdx;
	chat_t			*pCustomChat;	// custom chat data block
} cg_t;


typedef enum {
	HUD_GREEN,
	HUD_DARK_GREEN,
	HUD_WHITE,
	HUD_GREY,
	HUD_RED,
	HUD_DARK_RED,
	HUD_BLUE,
	HUD_DARK_BLUE,
	HUD_YELLOW,
	HUD_MAGENTA,
	HUD_CYAN,
	HUD_ORANGE,
	HUD_MAX
} HUDColors_t;

typedef enum {
	HR_GUIDED_WHITE,
	HR_TRACKING_FRIEND,
	HR_TRACKING_ENEMY,
	HR_LOCKED_FRIEND,
	HR_LOCKED_ENEMY,
	HR_GUIDED,
	HR_UNGUIDED_AND_GUN,
	HR_BOMB,
	HR_TARGET_FRIEND,
	HR_TARGET_ENEMY,
	HR_MAX
} HUDReticles_t;

// colors
extern vec4_t		HUDColors[HUD_MAX];

// all of the model, shader, and sound references that are
// loaded at gamestate time are stored in cgMedia_t
// Other media that can be tied to clients, weapons, or items are
// stored in the clientInfo_t, itemInfo_t, weaponInfo_t, and powerupInfo_t
typedef struct {
	qhandle_t	charsetShader;
	qhandle_t	charsetProp;
	qhandle_t	charsetPropGlow;
	qhandle_t	charsetPropB;
	qhandle_t	whiteShader;

	qhandle_t	redFlagModel;
	qhandle_t	blueFlagModel;
	qhandle_t	redFlagShader[3];
	qhandle_t	blueFlagShader[3];

	qhandle_t	teamStatusBar;

	qhandle_t	friendShader;

	qhandle_t	balloonShader;

	qhandle_t	particleShader;

	qhandle_t	connectionShader;

	qhandle_t	selectShader;
	qhandle_t	crosshairShader[NUM_CROSSHAIRS];
	qhandle_t	lagometerShader;
	qhandle_t	backTileShader;
	qhandle_t	noammoShader;

	qhandle_t	smokePuffShader;
	qhandle_t	smokePuffRageProShader;

	qhandle_t	numberShaders[11];

	qhandle_t	shadowMarkShader[MF_MAX_CATEGORIES];

	// wall mark shaders
	qhandle_t	bulletMarkShader;
	qhandle_t	burnMarkShader;

	// MFQ3 effect models
	qhandle_t	vapor;
	qhandle_t	vaporBig;
	qhandle_t	vaporShader;
	qhandle_t	afterburner[AB_MAX];
	qhandle_t	bulletTracer;
	qhandle_t	pilot;
	qhandle_t	pilotHead;
	qhandle_t	pilotSeat;
	qhandle_t	pilotWW2;
	qhandle_t	pilotHeadWW2;
	qhandle_t	pilotSeatWW2;
	qhandle_t	missilePuffShader;
	qhandle_t	flarePuffShader;
	qhandle_t   railCoreShader;
	qhandle_t	missileTrail2Shader;
	qhandle_t	nukePuffShader;
	qhandle_t	nukeCloud[NUKE_CLOUD_FRAMES];

	qhandle_t	reticle[CH_NUMCROSSHAIRS];

	// MFQ3 effect shaders
	qhandle_t	vehicleMuzzleFlashModel;

	// MFQ3 HUD
	qhandle_t	HUDmain;
	qhandle_t	HUDradar;
	qhandle_t	HUDgps;
	qhandle_t	HUDext;
	qhandle_t	throttle[MF_THROTTLE_AFTERBURNER+1];
	qhandle_t	radarIcons[RD_MAX_ICONS];

	// MFQ3 NEW HUD
	qhandle_t	HUDheading;
	qhandle_t	HUDvaluebox;
	qhandle_t	HUDvaluebox2;
	qhandle_t	HUDind_h;
	qhandle_t	HUDind_v;
	qhandle_t	HUDind_v_r;
	qhandle_t	HUDcaret_h;
	qhandle_t	HUDcaret_v_g_l;
	qhandle_t	HUDcaret_v_r_l;
	qhandle_t	HUDmfd;
	qhandle_t	HUDspeed;
	qhandle_t	HUDrwr;
	qhandle_t	HUDalt;
	qhandle_t	HUDsolid;
	qhandle_t	HUDhealth[10];
	qhandle_t	HUDthrottle_1[10];
	qhandle_t	HUDthrottle_2_1[10];
	qhandle_t	HUDthrottle_2_2[10];
	qhandle_t	HUDthrottle_1_ab[15];
	qhandle_t	HUDthrottle_2_1_ab[15];
	qhandle_t	HUDthrottle_2_2_ab[15];
	qhandle_t	HUDthrottle_rev[5];
	qhandle_t	HUDhealthtext;
	qhandle_t	HUDthrottletext;
	qhandle_t	HUDalphabet;
	qhandle_t	HUDnumberline;
	qhandle_t	HUDreticles[HR_MAX];
	
	// IGME
	qhandle_t	IGME_selector;
	qhandle_t	IGME_waypoint;
	qhandle_t	IGME_waypoint2;

	// weapon effect models
	qhandle_t	bulletFlashModel;
	qhandle_t	dishFlashModel;

	// weapon effect shaders
	qhandle_t	bulletExplosionShader;
	qhandle_t	rocketExplosionShader[2];
	qhandle_t	grenadeExplosionShader;

	// Nuke
	qhandle_t	nukeEffectModel;
	qhandle_t	nukeShockWave;
	qhandle_t	nukeCloudModel;
	sfxHandle_t nukeExplodeSound;
	sfxHandle_t nukeImplodeSound;
	sfxHandle_t nukeFarSound;

	// gib explosions
	sfxHandle_t	gibSound;
	qhandle_t	gibAbdomen;
	qhandle_t	gibArm;
	qhandle_t	gibChest;
	qhandle_t	gibFist;
	qhandle_t	gibFoot;
	qhandle_t	gibForearm;
	qhandle_t	gibIntestine;
	qhandle_t	gibLeg;
	qhandle_t	gibSkull;
	qhandle_t	gibBrain;

	// sounds
	sfxHandle_t	sfx_ric1;
	sfxHandle_t	sfx_ric2;
	sfxHandle_t	sfx_ric3;
	sfxHandle_t	sfx_rockexp;

	sfxHandle_t	respawnSound;
	sfxHandle_t talkSound;
	sfxHandle_t fallSound;

	sfxHandle_t oneMinuteSound;
	sfxHandle_t fiveMinuteSound;
	sfxHandle_t suddenDeathSound;

	sfxHandle_t threeFragSound;
	sfxHandle_t twoFragSound;
	sfxHandle_t oneFragSound;

	sfxHandle_t hitSound;
	sfxHandle_t hitTeamSound;
	sfxHandle_t impressiveSound;
	sfxHandle_t excellentSound;
	sfxHandle_t deniedSound;
	sfxHandle_t assistSound;
	sfxHandle_t defendSound;

	sfxHandle_t takenLeadSound;
	sfxHandle_t tiedLeadSound;
	sfxHandle_t lostLeadSound;

	sfxHandle_t watrInSound;
	sfxHandle_t watrOutSound;
	sfxHandle_t watrUnSound;

	// teamplay sounds
	sfxHandle_t captureAwardSound;
	sfxHandle_t redScoredSound;
	sfxHandle_t blueScoredSound;
	sfxHandle_t redLeadsSound;
	sfxHandle_t blueLeadsSound;
	sfxHandle_t teamsTiedSound;

	sfxHandle_t	captureYourTeamSound;
	sfxHandle_t	captureOpponentSound;
	sfxHandle_t	returnYourTeamSound;
	sfxHandle_t	returnOpponentSound;
	sfxHandle_t	takenYourTeamSound;
	sfxHandle_t	takenOpponentSound;

	sfxHandle_t redFlagReturnedSound;
	sfxHandle_t blueFlagReturnedSound;
	sfxHandle_t	enemyTookYourFlagSound;
	sfxHandle_t	enemyTookTheFlagSound;
	sfxHandle_t yourTeamTookEnemyFlagSound;
	sfxHandle_t yourTeamTookTheFlagSound;
	sfxHandle_t	youHaveFlagSound;
	sfxHandle_t yourBaseIsUnderAttackSound;
	sfxHandle_t holyShitSound;

	// tournament sounds
	sfxHandle_t	count3Sound;
	sfxHandle_t	count2Sound;
	sfxHandle_t	count1Sound;
	sfxHandle_t	countFightSound;
	sfxHandle_t	countPrepareSound;

	qhandle_t cursor;

	sfxHandle_t	n_healthSound;

	// MFQ3
	sfxHandle_t engineJet;
	sfxHandle_t	engineJetAB;
	sfxHandle_t engineProp;
	sfxHandle_t planeDeath[NUM_EXPLOSION_SOUNDS];
	sfxHandle_t engineTank[NUM_TANKSOUNDS];
	sfxHandle_t	engineHelo;

} cgMedia_t;


enum CameraAdjust {
	CAMADJ_NONE,
	CAMADJ_INOUT,
	CAMADJ_UPDOWN,
	MAX_CAMERA_ADJUSTS
};

// IGME

enum IGME_OBJECT_TYPES {
	IGMEO_VEHICLE,
	IGMEO_WAYPOINT
};

typedef struct {
	bool		active;
	bool		selected;
	vec3_t			origin;
} IGME_waypoint_t;

typedef struct {
	bool		active;
	bool		selected;
	bool		groundInstallation;
	int				vehidx;
	vec3_t			angles;
	vec3_t			origin;
	vec3_t			selectorScale;
	IGME_waypoint_t waypoints[IGME_MAX_WAYPOINTS];
} IGME_vehicle_t;

typedef struct {
	IGME_vehicle_t	vehicles[IGME_MAX_VEHICLES];
	int				selectionTime;
	int				numSelections;
	int				numWptSelections;
	bool		dragmode;
	bool		waypointmode;
	bool		showAllWaypoints;
	IGME_vehicle_t	copyBufferVehicle;
	bool		copyBufferVehicleUsed;
	IGME_waypoint_t	copyBufferRoute[IGME_MAX_WAYPOINTS];
	bool		copyBufferRouteUsed;
} IGME_data_t;

// end IGME

// MFQ3 vehicle drawing
typedef struct {
    int					vehicleIndex;
	float				speed;
	vec3_t				axis[3];
	vec3_t				origin;
	vec3_t				angles;
	int					ONOFF;
	completeLoadout_t*	usedLoadout;
	int					numWeaponsOnMount[32];
	int					throttle;
	int					entityNum;
	int					flashWeaponIndex;
	bool			drawMuzzleFlash;
} BasicDrawInfo_t;

typedef struct {
    BasicDrawInfo_t		basicInfo;
	int					controlFrame;
	int					gearFrame;
	int					bayFrame;
	int					brakeFrame;
	int					cockpitFrame;
	int					bodyFrame;
	float				swingAngle;
} DrawInfo_Plane_t;

typedef struct {
    BasicDrawInfo_t		basicInfo;
	float				turretAngle;
	float				gunAngle;
	float				wheelAngle;
	float				wheelDirection;
} DrawInfo_GV_t;

typedef struct {
    BasicDrawInfo_t		basicInfo;
	float				turretAngle[4];
	float				gunAngle[4];
} DrawInfo_Boat_t;

typedef struct {
    BasicDrawInfo_t		basicInfo;
	float				turretAngle[4];
	float				gunAngle[4];
} DrawInfo_Helo_t;

typedef struct {
    BasicDrawInfo_t		basicInfo;
	int					anim;
	int					torsoFrame;
	int					legsFrame;
	int					torsoTime;
	int					legsTime;
	int					lastLegsAngle;
	int					lastTorsoAngle;
	int					weaponIndex;
} DrawInfo_LQM_t;

typedef struct {
	BasicDrawInfo_t		basicInfo;
	int					upgrades;
	float				turretAngle;
	float				gunAngle;
} DrawInfo_GI_t;

// end MFQ3 vehicle drawing

// The client game static (cgs) structure hold everything
// loaded or calculated from the gamestate.  It will NOT
// be cleared when a tournement restart is done, allowing
// all clients to begin playing instantly
typedef struct {
	gameState_t		gameState;			// gamestate from server
	glconfig_t		glconfig;			// rendering configuration
	float			screenXScale;		// derived from glconfig
	float			screenYScale;
	float			screenXBias;

	int				serverCommandSequence;	// reliable command stream counter
	int				processedSnapshotNum;// the number of snapshots cgame has requested

	int				localServer;		// detected on startup by checking sv_running

	// parsed from serverinfo
	gametype_t		gametype;
	int				dmflags;
	int				teamflags;
	int				fraglimit;
	int				capturelimit;
	int				timelimit;
	int				maxclients;
	char			mapname[MAX_QPATH];
	char			redTeam[MAX_QPATH];
	char			blueTeam[MAX_QPATH];

	int				voteTime;
	int				voteYes;
	int				voteNo;
	bool		voteModified;			// beep whenever changed
	char			voteString[MAX_STRING_TOKENS];

	int				teamVoteTime[2];
	int				teamVoteYes[2];
	int				teamVoteNo[2];
	bool		teamVoteModified[2];	// beep whenever changed
	char			teamVoteString[2][MAX_STRING_TOKENS];

	int				levelStartTime;

	int				scores1, scores2;		// from configstrings
	int				redflag, blueflag;		// flag status from configstrings
	int				flagStatus;

	bool  newHud;

	// mfq3
	unsigned long	gameset;

	//
	// locally derived information from gamestate
	//
	qhandle_t		gameModels[MAX_MODELS];
	sfxHandle_t		gameSounds[MAX_SOUNDS];

	int				numInlineModels;
	qhandle_t		inlineDrawModel[MAX_MODELS];
	vec3_t			inlineModelMidpoints[MAX_MODELS];

	clientInfo_t	clientinfo[MAX_CLIENTS];

	// teamchat width is *3 because of embedded color codes
	char			teamChatMsgs[TEAMCHAT_HEIGHT][TEAMCHAT_WIDTH*3+1];
	int				teamChatMsgTimes[TEAMCHAT_HEIGHT];
	int				teamChatPos;
	int				teamLastChatPos;

	int cursorX;
	int cursorY;
	bool eventHandling;
	bool mouseCaptured;
	bool sizingHud;
	void *capturedItem;
	qhandle_t activeCursor;

	// orders
	int currentOrder;
	bool orderPending;
	int orderTime;
	int acceptOrderTime;
	int acceptTask;
	int acceptLeader;

	// media
	cgMedia_t		media;

	// IGME
	IGME_data_t		IGME;

} cgs_t;

//==============================================================================

//extern	vmCvar_t		cg_pmove_fixed;
//extern	vmCvar_t		cg_drawCrosshair;

extern	cgs_t			cgs;
extern	cg_t			cg;
extern	centity_t		cg_entities[MAX_GENTITIES];
extern	weaponInfo_t	cg_weapons[WI_MAX];
extern	itemInfo_t		cg_items[MAX_ITEMS];
extern	markPoly_t		cg_markPolys[MAX_MARK_POLYS];

// MFQ3
extern	completeLoadout_t cg_loadouts[MAX_GENTITIES];
// -MFQ3

extern	vmCvar_t		cg_centertime;
extern	vmCvar_t		cg_runpitch;
extern	vmCvar_t		cg_runroll;
extern	vmCvar_t		cg_bobup;
extern	vmCvar_t		cg_bobpitch;
extern	vmCvar_t		cg_bobroll;
extern	vmCvar_t		cg_swingSpeed;
extern	vmCvar_t		cg_shadows;
extern	vmCvar_t		cg_drawTimer;
extern	vmCvar_t		cg_drawFPS;
extern	vmCvar_t		cg_drawParticleStats;
extern	vmCvar_t		cg_drawSnapshot;
extern	vmCvar_t		cg_draw3dIcons;
extern	vmCvar_t		cg_drawIcons;
extern	vmCvar_t		cg_drawCrosshairNames;
extern	vmCvar_t		cg_drawRewards;
extern	vmCvar_t		cg_drawTeamOverlay;
extern	vmCvar_t		cg_teamOverlayUserinfo;
extern	vmCvar_t		cg_crosshairX;
extern	vmCvar_t		cg_crosshairY;
extern	vmCvar_t		cg_crosshairSize;
extern	vmCvar_t		cg_drawStatus;
extern	vmCvar_t		cg_draw2D;
extern	vmCvar_t		cg_debugPosition;
extern	vmCvar_t		cg_debugEvents;
extern	vmCvar_t		cg_missileEffect;	// 0 Default, 1 White puffs, 2 "Rail trail"
extern	vmCvar_t		cg_railTrailTime;
extern	vmCvar_t		cg_errorDecay;
extern	vmCvar_t		cg_nopredict;
extern	vmCvar_t		cg_showmiss;
extern	vmCvar_t		cg_addMarks;
extern	vmCvar_t		cg_gun_frame;
extern	vmCvar_t		cg_gun_x;
extern	vmCvar_t		cg_gun_y;
extern	vmCvar_t		cg_gun_z;
extern	vmCvar_t		cg_drawGun;
extern	vmCvar_t		cg_viewsize;
extern	vmCvar_t		cg_ignore;
extern	vmCvar_t		cg_simpleItems;
extern	vmCvar_t		cg_fov;
extern	vmCvar_t		cg_zoomFov;
extern	vmCvar_t		cg_thirdPersonRange;
extern	vmCvar_t		cg_thirdPersonAngle;
extern	vmCvar_t		cg_thirdPerson;
extern	vmCvar_t		cg_stereoSeparation;
extern	vmCvar_t		cg_lagometer;
extern	vmCvar_t		cg_synchronousClients;
extern	vmCvar_t		cg_teamChatTime;
extern	vmCvar_t		cg_teamChatHeight;
extern	vmCvar_t		cg_stats;
extern	vmCvar_t 		cg_forceModel;
extern	vmCvar_t 		cg_buildScript;
extern	vmCvar_t		cg_paused;
extern	vmCvar_t		cg_freelook;
extern	vmCvar_t		cg_advanced;
extern	vmCvar_t		cg_predictItems;
extern	vmCvar_t		cg_drawFriend;
extern	vmCvar_t		cg_teamChatsOnly;
extern	vmCvar_t		cg_smoothClients;
extern	vmCvar_t		pmove_fixed;
extern	vmCvar_t		pmove_msec;
extern	vmCvar_t		cg_timescaleFadeEnd;
extern	vmCvar_t		cg_timescaleFadeSpeed;
extern	vmCvar_t		cg_timescale;
extern	vmCvar_t		cg_cameraMode;
extern  vmCvar_t		cg_smallFont;
extern  vmCvar_t		cg_bigFont;
extern	vmCvar_t		cg_noTaunt;

// MFQ3+
extern	vmCvar_t		cg_vehicle;
extern	vmCvar_t		cg_nextVehicle;
extern	vmCvar_t		cg_thirdPersonHeight;
extern	vmCvar_t		cg_radarTargets;
extern  vmCvar_t		cg_tracer;
extern  vmCvar_t		cg_smoke;

extern	vmCvar_t		cg_oldHUD;

extern	vmCvar_t		hud_heading;
extern	vmCvar_t		hud_speed;
extern	vmCvar_t		hud_altitude;
extern	vmCvar_t		hud_mfd;
extern	vmCvar_t		hud_mfd2;
extern	vmCvar_t		hud_center;
extern  vmCvar_t		hud_health;
extern	vmCvar_t		hud_throttle;
extern	vmCvar_t		hud_weapons;

extern	vmCvar_t		hud_color;
extern	vmCvar_t		mfd_color;
extern	vmCvar_t		mfd1_defaultpage;
extern	vmCvar_t		mfd2_defaultpage;

extern	vmCvar_t		m1cp_page0;
extern	vmCvar_t		m1cp_page1;
extern	vmCvar_t		m1cp_page2;
extern	vmCvar_t		m1cp_page3;
extern	vmCvar_t		m1cp_page4;
extern	vmCvar_t		m1cp_page5;
extern	vmCvar_t		m2cp_page0;
extern	vmCvar_t		m2cp_page1;
extern	vmCvar_t		m2cp_page2;
extern	vmCvar_t		m2cp_page3;
extern	vmCvar_t		m2cp_page4;
extern	vmCvar_t		m2cp_page5;

extern	vmCvar_t		cg_fxQuality;
extern	vmCvar_t		cg_consoleTextStyle;

extern	vmCvar_t		cg_shadowDebug;

// MFQ3-

//
// cg_main.c
//
const char *CG_ConfigString( int index );
const char *CG_Argv( int arg );

void QDECL CG_Printf( const char *msg, ... );
//void QDECL CG_Error( const char *msg, ... );

void CG_StartMusic( void );

void CG_UpdateCvars( void );

int CG_CrosshairPlayer( void );
int CG_LastAttacker( void );
void CG_LoadMenus(const char *menuFile);
void CG_KeyEvent(int key, bool down);
void CG_MouseEvent(int x, int y);
void CG_EventHandling(int type);
void CG_RankRunFrame( void );
void CG_SetScoreSelection(void *menu);
score_t *CG_GetSelectedScore();
void CG_BuildSpectatorString();
int CG_FeederCount(float feederID);

void CG_AddRegisterMessage(const char * s);

//
// cg_util.c
//
float CG_Cvar_Get( const char * cvar );
unsigned long MF_CG_GetGameset( bool asEnum );

//
// cg_testmodel.c
//
void CG_TestPlaneCmd_f(void);
void CG_TestGVCmd_f(void);
void CG_TestVehicle(long cat);
void CG_AddTestVehicle (void);
void CG_AddTestModel (void);
void CG_TestModel_f (void);
void CG_TestGun_f (void);
void CG_TestModelNextFrame_f (void);
void CG_TestModelPrevFrame_f (void);
void CG_TestModelNextSkin_f (void);
void CG_TestModelPrevSkin_f (void);


//
// cg_view.c
//

void CG_ZoomDown_f( void );
void CG_ZoomUp_f( void );
void CG_AddBufferedSound( sfxHandle_t sfx);
void CG_DrawActiveFrame( int serverTime, stereoFrame_t stereoView, bool demoPlayback );
bool CG_WorldToScreenCoords( vec3_t worldPoint, int * pX, int * pY, bool virtualXY );

//
// cg_drawtools.c
//
void CG_DrawProgressBar( rectDef_t *rect, vec4_t color, vec4_t textcolor, float scale, int textStyle, float progress );
void CG_AdjustFrom640( float *x, float *y, float *w, float *h );
void CG_FillRect( float x, float y, float width, float height, const float *color );
void CG_DrawPic( float x, float y, float width, float height, qhandle_t hShader );
void CG_DrawString( float x, float y, const char *string, 
				   float charWidth, float charHeight, const float *modulate );


void CG_DrawStringExt( int x, int y, const char *string, const float *setColor, 
		bool forceColor, bool shadow, int charWidth, int charHeight, int maxChars );
void CG_DrawBigString( int x, int y, const char *s, float alpha );
void CG_DrawBigStringColor( int x, int y, const char *s, vec4_t color );
void CG_DrawSmallString( int x, int y, const char *s, float alpha );
void CG_DrawSmallStringColor( int x, int y, const char *s, vec4_t color );

int CG_DrawStrlen( const char *str );

float	*CG_FadeColor( int startMsec, int totalMsec );
float *CG_TeamColor( int team );
void CG_TileClear( void );

void UI_DrawProportionalString( int x, int y, const char* str, int style, vec4_t color );
void CG_DrawRect( float x, float y, float width, float height, float size, const float *color );
void CG_DrawSides(float x, float y, float w, float h, float size);
void CG_DrawTopBottom(float x, float y, float w, float h, float size);

bool CG_GenericShadow( BasicDrawInfo_t *drawInfo, float *shadowPlane );

void CG_ResetReticles( void );
void CG_AddReticleEntityToScene( refEntity_t * pReticle, centity_t * pTarget );
void CG_Draw_Reticles();
//void CG_Draw_HUD_Label( int x, int y, char * pText, float alpha );
localEntity_t * CG_Generic_Smoke( centity_t * cent, vec3_t smokePosition, int density );

//
// cg_draw.c, cg_newDraw.c
//
extern	int sortedTeamPlayers[TEAM_MAXOVERLAY];
extern	int	numSortedTeamPlayers;
extern	int drawTeamOverlayModificationCount;
extern  char systemChat[256];
extern  char teamChat1[256];
extern  char teamChat2[256];

void CG_AddLagometerFrameInfo( void );
void CG_AddLagometerSnapshotInfo( snapshot_t *snap );
void CG_CenterPrint( const char *str, int y, int charWidth );
void CG_DrawActive( stereoFrame_t stereoView );
void CG_DrawFlagModel( float x, float y, float w, float h, int team, bool force2D );
void CG_DrawTeamBackground( int x, int y, int w, int h, float alpha, int team );
void CG_OwnerDraw(float x, float y, float w, float h, float text_x, float text_y, int ownerDraw, int ownerDrawFlags, 
				  int align, float special, float scale, const vec4_t color, qhandle_t shader, int textStyle, 
				  itemDef_t * item );
void CG_Text_Paint(float x, float y, float scale, const vec4_t color, const char *text, float adjust, int limit, int style);
int CG_Text_Width(const char *text, float scale, int limit);
int CG_Text_Height(const char *text, float scale, int limit);
void CG_SelectPrevPlayer();
void CG_SelectNextPlayer();
float CG_GetValue(int ownerDraw);
bool CG_OwnerDrawVisible(int flags);
void CG_RunMenuScript(char **args);
void CG_ShowResponseHead();
void CG_SetPrintString(int type, const char *p);
void CG_InitTeamChat();
void CG_GetTeamColor(vec4_t *color);
const char *CG_GetGameStatusText();
const char *CG_GetKillerText();
void CG_Draw3DModel( float x, float y, float w, float h, qhandle_t model, qhandle_t skin, vec3_t origin, vec3_t angles );
void CG_Text_PaintChar(float x, float y, float width, float height, float scale, float s, float t, float s2, float t2, qhandle_t hShader);
void CG_CheckOrderPending();
const char *CG_GameTypeString();
bool CG_YourTeamHasFlag();
bool CG_OtherTeamHasFlag();
qhandle_t CG_StatusHandle(int task);

void CG_DrawStatusBar_MFQ3();
void CG_DrawStatusBar_MFQ3_new();

void CG_Add_Console_Line( char * pText );
bool CG_NewHUDActive( void );

//
// cg_player.c
//
void CG_NewClientInfo( int clientNum );
sfxHandle_t	CG_CustomSound( int clientNum, const char *soundName );
void CG_TrailItem( centity_t *cent, qhandle_t hModel );	// ditto

//
// cg_vehicle.c
//
void CG_CacheVehicles();
void CG_Vehicle( centity_t *cent );
void CG_RegisterVehicle( clientInfo_t *ci );
void CG_VehicleObituary( entityState_t *ent );
void CG_VehicleMuzzleFlash( int weaponIdx, const refEntity_t *parent, qhandle_t parentModel, int idx );
void CG_Misc_Vehicle( centity_t *cent );
void CG_InitShadows( void );


//
// cg_vehicledraw.c
//
void CG_DrawPlane(DrawInfo_Plane_t* drawInfo);
void CG_DrawGV(DrawInfo_GV_t* drawInfo);
void CG_DrawBoat(DrawInfo_Boat_t* drawInfo);
void CG_DrawHelo(DrawInfo_Helo_t* drawInfo);
void CG_DrawLQM(DrawInfo_LQM_t* drawInfo);
void CG_DrawGI(DrawInfo_GI_t* drawInfo);

//
// cg_plane.c
//
void CG_Plane( centity_t *cent, clientInfo_t *ci );
void CG_RegisterPlane( clientInfo_t *ci );
void CG_PlaneObituary( entityState_t *ent, clientInfo_t *ci );

//
// cg_groundvehicle.c
//
void CG_GroundVehicle( centity_t *cent, clientInfo_t *ci );
void CG_RegisterGroundVehicle( clientInfo_t *ci );
void CG_GroundVehicleObituary( entityState_t *ent, clientInfo_t *ci );


//
// cg_helo.c
//
void CG_Helo( centity_t *cent, clientInfo_t *ci );
void CG_RegisterHelo( clientInfo_t *ci );
void CG_HeloObituary( entityState_t *ent, clientInfo_t *ci );


//
// cg_lqm.c
//
void CG_LQM( centity_t *cent, clientInfo_t *ci );
void CG_RegisterLQM( clientInfo_t *ci );
void CG_LQMObituary( entityState_t *ent, clientInfo_t *ci );


//
// cg_boat.c
//
void CG_Boat( centity_t *cent, clientInfo_t *ci );
void CG_RegisterBoat( clientInfo_t *ci );
void CG_BoatObituary( entityState_t *ent, clientInfo_t *ci );


//
// cg_predict.c
//
void CG_BuildSolidList( void );
int	CG_PointContents( const vec3_t point, int passEntityNum );
void CG_Trace( trace_t *result, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, 
					 int skipNumber, int mask, bool capsule = true );
void CG_PredictPlayerState( void );


//
// cg_events.c
//
void CG_CheckEvents( centity_t *cent );
const char	*CG_PlaceString( int rank );
void CG_EntityEvent( centity_t *cent, vec3_t position );
void CG_PainEvent( centity_t *cent, int health );


//
// cg_ents.c
//
void CG_SetEntitySoundPosition( centity_t *cent );
void CG_AddPacketEntities( void );
void CG_Beam( centity_t *cent );
void CG_AdjustPositionForMover( const vec3_t in, int moverNum, int fromTime, int toTime, vec3_t out );

void CG_PositionEntityOnTag( refEntity_t *entity, const refEntity_t *parent, 
							qhandle_t parentModel, char *tagName );
void CG_PositionRotatedEntityOnTag( refEntity_t *entity, const refEntity_t *parent, 
							qhandle_t parentModel, char *tagName );



//
// cg_weapons.c
//
void CG_NextWeapon_f( void );
void CG_PrevWeapon_f( void );
void CG_Weapon_f( void );

void CG_RegisterWeapons();
void CG_RegisterItemVisuals( int itemNum );

void CG_FireWeapon( centity_t *cent );
void CG_FireMachinegun( centity_t *cent, bool main );
void CG_MissileHitWall( int weapon, int clientNum, vec3_t origin, vec3_t dir, impactSound_t soundType );
void CG_GenericExplosion( vec3_t origin, int type );
void CG_VehicleHit( vec3_t origin, int damage );
void CG_Bullet( vec3_t origin, int sourceEntityNum, vec3_t normal, int fleshEntityNum );

//
// cg_marks.c
//
void	CG_InitMarkPolys( void );
void	CG_AddMarks( void );

void	CG_ImpactMark( qhandle_t markShader, 
				    const vec3_t origin, const vec3_t dir, 
					float orientation, 
				    float r, float g, float b, float a, 
					bool alphaFade, 
					float radius, bool temporary );

void	CG_ImpactMarkEx( qhandle_t markShader, const vec3_t origin, const vec3_t dir, 
						 float orientation, float red, float green, float blue, float alpha,
						 bool alphaFade, float xRadius, float yRadius, bool temporary );

//
// cg_localents.c
//
void	CG_InitLocalEntities( void );
localEntity_t	*CG_AllocLocalEntity( void );
void	CG_AddLocalEntities( void );

//
// cg_effects.c
//
localEntity_t *CG_SmokePuff( const vec3_t p, 
				   const vec3_t vel, 
				   float radius,
				   float r, float g, float b, float a,
				   float duration,
				   int startTime,
				   int fadeInTime,
				   int leFlags,
				   qhandle_t hShader );

localEntity_t *CG_MakeExplosion( vec3_t origin, vec3_t dir,
								qhandle_t hModel, qhandle_t shader, int offset, int duration,
								bool isSprite );

void CG_NukeEffect(  centity_t * cent, entityState_t * es );
void CG_FlakEffect(  centity_t * cent, entityState_t * es );
void CG_GibPlayer( vec3_t playerOrigin );

//
// cg_snapshot.c
//
void CG_ProcessSnapshots( void );

//
// cg_info.c
//
void CG_LoadingString( const char *s );
void CG_DrawInformation( void );

//
// cg_scoreboard.c
//
void CG_DrawOldTourneyScoreboard( void );

//
// cg_consolecmds.c
//
bool CG_ConsoleCommand( void );
void CG_InitConsoleCommands( void );

//
// cg_servercmds.c
//
void CG_ExecuteNewServerCommands( int latestSequence );
void CG_ParseServerinfo( void );
void CG_SetConfigValues( void );
void CG_ShaderStateChanged(void);

//
// cg_playerstate.c
//
void CG_Respawn( void );
void CG_TransitionPlayerState( playerState_t *ps, playerState_t *ops );
void CG_CheckChangedPredictableEvents( playerState_t *ps );

//
// cg_missioneditor.c
//
IGME_vehicle_t* ME_SpawnVehicle( int idx );
void ME_SpawnGroundInstallation( int idx );
void ME_ExportToScript( const char* scriptname );
void ME_ImportScript( const char* scriptname );
void CG_Draw_IGME();
void ME_KeyEvent(int key, bool down);
void ME_MouseEvent(int x, int y);
void ME_Init_MissionEditor();

//===============================================

// system calls
int Sys_Milliseconds ();
void Cvar_Register( vmCvar_t *vmCvar, const char *varName, const char *defaultValue, int flags );
void Cvar_Update( vmCvar_t *vmCvar );
void Cvar_VariableStringBuffer( const char *var_name, char *buffer, int bufsize );
void Cvar_Set( const char *var_name, const char *value );
void Cvar_VariableStringBuffer( const char *var_name, char *buffer, int bufsize );
int	Cmd_Argc();
void Cmd_ArgvBuffer( int arg, char *buffer, int bufferLength );
void Cmd_ArgsBuffer( char *buffer, int bufferLength );
void Cbuf_AddText( const char *text );
void CL_AddCgameCommand( const char *cmdName );
void CL_AddReliableCommand( const char *cmd );
int FS_FOpenFileByMode( const char *qpath, fileHandle_t *f, fsMode_t mode );
void FS_FCloseFile( fileHandle_t f );
int FS_Read2( void *buffer, int len, fileHandle_t f );
int FS_Write( const void *buffer, int len, fileHandle_t h );
void SCR_UpdateScreen();
void CL_CM_LoadMap( const char *mapname );
int	CM_NumInlineModels();
clipHandle_t CM_InlineModel( int index );
clipHandle_t CM_TempBoxModel( const vec3_t mins, const vec3_t maxs, bool capsule );
int CM_PointContents( const vec3_t p, clipHandle_t model );
int	CM_TransformedPointContents( const vec3_t p, clipHandle_t model, const vec3_t origin, const vec3_t angles);
void CM_BoxTrace( trace_t *results, const vec3_t start, const vec3_t end,
				  const vec3_t mins, const vec3_t maxs,
				  clipHandle_t model, int brushmask, bool capsule ); 
void CM_TransformedBoxTrace( trace_t *results, const vec3_t start, const vec3_t end,
							 const vec3_t mins, const vec3_t maxs,
							 clipHandle_t model, int brushmask,
							 const vec3_t origin, const vec3_t angles, bool capsule );
void S_StartLocalSound( sfxHandle_t sfxHandle, int channelNum );
void S_StartSound(vec3_t origin, int entityNum, int entchannel, sfxHandle_t sfxHandle );
sfxHandle_t	S_RegisterSound( const char *name, bool compressed );
void S_ClearLoopingSounds( bool killall );
void S_AddLoopingSound( int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfxHandle );
void S_AddRealLoopingSound( int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfxHandle );
void S_StartBackgroundTrack( const char *intro, const char *loop );
void S_UpdateEntityPosition( int entityNum, const vec3_t origin );
void S_Respatialize( int entityNum, const vec3_t head, vec3_t axis[3], int inwater );
bool Key_IsDown( int keynum );
int Key_GetCatcher();
void Key_SetCatcher( int catcher );
void CL_GetGlconfig( glconfig_t *glconfig );
void CL_GetGameState( gameState_t *gs );
void CL_GetCurrentSnapshotNumber( int *snapshotNumber, int *serverTime );
bool CL_GetSnapshot( int snapshotNumber, snapshot_t *snapshot );
bool CL_GetServerCommand( int serverCommandNumber );
int CL_GetCurrentCmdNumber();
bool CL_GetUserCmd( int cmdNumber, usercmd_t *ucmd );
void CL_SetUserCmdValue( int userCmdValue, float sensitivityScale );




// aids for VM testing
void		testPrintInt( char *string, int i );
void		testPrintFloat( char *string, float f );


typedef enum {
  SYSTEM_PRINT,
  CHAT_PRINT,
  TEAMCHAT_PRINT
};


// cg_particles.c



#define MAX_WORLD_PARTICLES 		4096
#define	MAX_WORLD_EMITTERS			500
#define MAX_WORLD_SYSTEMS			200
#define MAX_PARTICLE_SHADERS		100




// Particle systems are nothing more than a collection of emitters
typedef struct particleSystem_s
{
	centity_t	* cent;

	vec3_t		pos;		// position of system  if cent != null
	// age properties
	bool		dead;
	long		birthtime;
	long		lifetime;
	long		lastthink;

} particleSystem_t;


typedef struct particle_s
{
	int	parentEmitter;
	
	// age properties
	bool		dead;
	long		birthtime;
	long		lifetime;
	long		lastthink;
	long		lastAttractionThink;
	
	// size
	float	radius;				// radius of particle
	long	growstart;			// when to start growing
	long 	growend;			// when to end growing
	float	growrate;			// how fast to grow
	
	// phsyics
	vec3_t		trDelta;
	vec3_t		trBase;
	vec3_t		trAttraction;
	float		maxspeed;
	float		accel;				// acceleration rate
	float		acceltime;			// how long to accelerate for
	float		rotation;			// current rotation
	float  		rotationvel;		// rotation velocity
	float		attractionSystem;	// attraction to particles of the same system in a world
	float 		attractionEmitter;	// attraction to particles of the same emitter in a world
	float		attractionWorld;	// attraction to all particles in the same world 
	float		attractionEntity;	// Attraction to entities;
	float		bounce;				// Bounce  1.0 = perfect bounce
	int			pm_type;			//
	bool		dieOnCollision;		//
	float		gravity;			// gravity value;
		
	// visual properties
	bool				reallight;		// whether or not to be affected by world lights
	qhandle_t			shaders[100];	// visual shader to use
	unsigned int 		shaderFPS;		// shader animation time; 0 = by age, > 0 by fps
	unsigned int		numShaders;		// number of shaders
	float				alpha;			// alpha transparacy of particle
	long 				alphaDecayStartTime;
	long				alphaDecayEndTime;
	float				alphaDecayStartValue;
	float				alphaDecayEndValue;
} particle_t;

// Particle emitters spawn New particles in a certain way
typedef struct particleEmitter_s
{
	bool					dead;

	long					birthtime;
	long					lifetime;
	long					lastthink;

	int						parentSytem;
	int						inheritVelocity;   // -1 = eject with opposite velocity, 0 = stationary, 1 = eject with same velocity

	particle_t				particleTemplate;
	
	vec3_t					ejectVelocity;

	vec3_t					accelVarianceMax;
	vec3_t					accelVarianceMin;
	float					rotationVariance;
	float					velVariance;
	float					maxspeedVariance;
	float					gravityVariance;
	float					bounceVariance;
	float					radiusVariance;
	float					lifetimeVariance;

	long					numEjectPerInterval;
	long					ejectInterval;
	long					lastEjectTime;
} particleEmitter_t;


// Particle worlds are nothing more than a collection of systems
typedef struct particleWorld_s					// Usualy only one world;
{
	particle_t			particles[MAX_WORLD_PARTICLES];
	particleSystem_t	systems[MAX_WORLD_SYSTEMS];
	particleEmitter_t	emitters[MAX_WORLD_EMITTERS];
	int					numParticles;
} particleWorld_t;

void CG_ParticleWorldRender(particleWorld_t * w);
void CG_ParticleWorldThink(particleWorld_t * w);
int CG_AddParticleEmitter(particleWorld_t * w, particleEmitter_t * e);
int CG_AddParticleSystem(particleWorld_t * w, particleSystem_t * s);
void CG_ParticleWorldInit(particleWorld_t * w);
void CG_ParticleThink(particleWorld_t * w, particle_t * p);
extern particleWorld_t pw;
extern refexport_t	refExport;


#endif // __CG_LOCAL_H__
