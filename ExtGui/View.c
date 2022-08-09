#include "Interface.h"

// # # # # # # # # # # # # # # # # # # # #
// # FLY                                 #
// # # # # # # # # # # # # # # # # # # # #

static void Camera_CalculateFly(Camera* cam) {
	Vec3f zero = Math_Vec3f_New(0, 0, 0);
	Vec3f upOffset = Math_Vec3f_New(0, 1, 0);
	Vec3f rOffset = Math_Vec3f_New(1, 0, 0);
	Vec3f atOffset = Math_Vec3f_New(0, 0, cam->dist);
	
	Matrix_Push();
	
	// Math_DelSmoothStepToS(&cam->vYaw, cam->yaw, 7, DegToBin(45), DegToBin(0.1));
	// Math_DelSmoothStepToS(&cam->vPitch, cam->pitch, 7, DegToBin(45), DegToBin(0.1));
	Matrix_Translate(cam->eye.x, cam->eye.y, cam->eye.z, MTXMODE_NEW);
	Matrix_RotateY_s(cam->yaw, MTXMODE_APPLY);
	Matrix_RotateX_s(cam->pitch, MTXMODE_APPLY);
	Matrix_RotateZ_s(cam->roll, MTXMODE_APPLY);
	
	Matrix_Translate(cam->vel.x, cam->vel.y, cam->vel.z, MTXMODE_APPLY);
	Matrix_MultVec3f(&zero, &cam->eye);
	
	Matrix_MultVec3f(&upOffset, &cam->up);
	Matrix_MultVec3f(&rOffset, &cam->right);
	cam->up = Math_Vec3f_Sub(cam->up, cam->eye);
	cam->right = Math_Vec3f_Sub(cam->right, cam->eye);
	Matrix_MultVec3f(&atOffset, &cam->at);
	
	Matrix_Pop();
}

static void Camera_FlyMode(View3D* this, Input* inputCtx) {
	Camera* cam = this->currentCamera;
	s16 pitch = Math_Vec3f_Pitch(cam->eye, cam->at);
	static s32 flip;
	static s32 init;
	
	if (this->cameraControl) {
		f32 step = 1.0f * cam->speedMod;
		f32 min = 0.01f * cam->speedMod;
		
		if (inputCtx->key[KEY_LEFT_SHIFT].hold) {
			step *= 4;
			Math_DelSmoothStepToF(&cam->speed, cam->speedMod * 4 * gDeltaTime, 0.25f, 1.00f, 0.1f);
		} else if (inputCtx->key[KEY_SPACE].hold) {
			step *= 8;
			Math_DelSmoothStepToF(&cam->speed, cam->speedMod * 8 * gDeltaTime, 0.25f, 1.00f, 0.1f);
		} else {
			Math_DelSmoothStepToF(&cam->speed, cam->speedMod * gDeltaTime, 0.25f, 1.00f, 0.1f);
		}
		
		if (inputCtx->key[KEY_A].hold || inputCtx->key[KEY_D].hold) {
			if (inputCtx->key[KEY_A].hold)
				Math_DelSmoothStepToF(&cam->vel.x, cam->speed, 0.25f, step, min);
			if (inputCtx->key[KEY_D].hold)
				Math_DelSmoothStepToF(&cam->vel.x, -cam->speed, 0.25f, step, min);
		} else {
			Math_DelSmoothStepToF(&cam->vel.x, 0, 0.25f, step, min);
		}
		
		if (inputCtx->key[KEY_W].hold || inputCtx->key[KEY_S].hold) {
			if (inputCtx->key[KEY_W].hold)
				Math_DelSmoothStepToF(&cam->vel.z, cam->speed, 0.25f, step, min);
			if (inputCtx->key[KEY_S].hold)
				Math_DelSmoothStepToF(&cam->vel.z, -cam->speed, 0.25f, step, min);
		} else {
			Math_DelSmoothStepToF(&cam->vel.z, 0, 0.25f, step, min);
		}
		
		if (inputCtx->key[KEY_Q].hold || inputCtx->key[KEY_E].hold) {
			if (inputCtx->key[KEY_Q].hold)
				Math_DelSmoothStepToF(&cam->vel.y, -cam->speed, 0.25f, step, min);
			if (inputCtx->key[KEY_E].hold)
				Math_DelSmoothStepToF(&cam->vel.y, cam->speed, 0.25f, step, min);
		} else {
			Math_DelSmoothStepToF(&cam->vel.y, 0, 0.25f, step, min);
		}
		
		if (Input_GetMouse(inputCtx, MOUSE_L)->hold) {
			if (init == 0)
				flip = Abs(cam->pitch - pitch) < DegToBin(0.1) ? 1 : -1;
			init = 1;
			cam->pitch = (s32)(cam->pitch + inputCtx->mouse.vel.y * 55.5f);
			cam->yaw = (s32)(cam->yaw - inputCtx->mouse.vel.x * 55.5f * flip);
		} else
			init = 0;
	} else {
		init = 0;
		Math_DelSmoothStepToF(&cam->speed, 0.5f, 0.5f, 1.00f, 0.1f);
		Math_DelSmoothStepToF(&cam->vel.x, 0.0f, 0.5f, 1.00f, 0.1f);
		Math_DelSmoothStepToF(&cam->vel.z, 0.0f, 0.5f, 1.00f, 0.1f);
		Math_DelSmoothStepToF(&cam->vel.y, 0.0f, 0.5f, 1.00f, 0.1f);
	}
	
	Camera_CalculateFly(cam);
}

// # # # # # # # # # # # # # # # # # # # #
// # ORBIT                               #
// # # # # # # # # # # # # # # # # # # # #

static void Camera_CalculateOrbit(Camera* cam) {
	Vec3f zero = Math_Vec3f_New(0, 0, 0);
	Vec3f upOffset = Math_Vec3f_New(0, 1, 0);
	Vec3f rOffset = Math_Vec3f_New(1, 0, 0);
	Vec3f atOffset = Math_Vec3f_New(0, 0, -cam->dist);
	
	Matrix_Push();
	Matrix_Translate(cam->at.x, cam->at.y, cam->at.z, MTXMODE_NEW);
	
	// cam->vYaw = cam->yaw;
	// cam->vPitch = cam->pitch;
	Matrix_RotateY_s(cam->yaw, MTXMODE_APPLY);
	Matrix_RotateX_s(cam->pitch, MTXMODE_APPLY);
	Matrix_RotateZ_s(cam->roll, MTXMODE_APPLY);
	
	Matrix_Translate(cam->offset.x, cam->offset.y, cam->offset.z, MTXMODE_APPLY);
	cam->offset = Math_Vec3f_New(0, 0, 0);
	Matrix_MultVec3f(&zero, &cam->at);
	
	Matrix_MultVec3f(&upOffset, &cam->up);
	Matrix_MultVec3f(&rOffset, &cam->right);
	cam->up = Math_Vec3f_Sub(cam->up, cam->at);
	cam->right = Math_Vec3f_Sub(cam->right, cam->at);
	Matrix_MultVec3f(&atOffset, &cam->eye);
	
	Matrix_Pop();
}

static void Camera_OrbitMode(View3D* this, Input* inputCtx) {
	Camera* cam = this->currentCamera;
	f32 distMult = (cam->dist * 0.2);
	f32 disdiff = fabsf(cam->dist - cam->targetDist);
	f32 fovDiff = fabsf(this->fovy - this->fovyTarget);
	
	if (this->cameraControl) {
		if (inputCtx->key[KEY_LEFT_CONTROL].hold && inputCtx->mouse.clickMid.hold && inputCtx->mouse.scrollY == 0)
			cam->targetDist += inputCtx->mouse.vel.y;
		
		if (inputCtx->mouse.clickMid.hold || inputCtx->mouse.scrollY || disdiff || fovDiff) {
			if (inputCtx->key[KEY_LEFT_CONTROL].hold && inputCtx->mouse.scrollY) {
				cam->targetDist = cam->dist;
				this->fovyTarget = Clamp(this->fovyTarget * (1.0 + (inputCtx->mouse.scrollY / 20)), 30, 120);
				fovDiff = -this->fovy;
			} else {
				if (inputCtx->mouse.scrollY) {
					cam->targetDist -= inputCtx->mouse.scrollY * distMult * 0.75f;
				}
				
				cam->targetDist = ClampMin(cam->targetDist, 1.0f);
			}
			
			if (inputCtx->mouse.clickMid.hold) {
				if (inputCtx->key[KEY_LEFT_SHIFT].hold) {
					cam->offset.y += inputCtx->mouse.vel.y * distMult * 0.01f;
					cam->offset.x += inputCtx->mouse.vel.x * distMult * 0.01f;
				} else if (inputCtx->key[KEY_LEFT_CONTROL].hold == false) {
					cam->yaw -= inputCtx->mouse.vel.x * 67;
					cam->pitch += inputCtx->mouse.vel.y * 67;
				}
			}
		}
	}
	
	Math_DelSmoothStepToF(&cam->dist, cam->targetDist, 0.25f, 5.0f * distMult, 0.01f * distMult);
	if (fovDiff) {
		f32 f = -this->fovy;
		Math_DelSmoothStepToF(&this->fovy, this->fovyTarget, 0.25, 5.25f, 0.0f);
		f += this->fovy;
		
		cam->offset.z += f * 8.f * (1.0f - this->fovy / 150);
	}
	
	Camera_CalculateOrbit(cam);
}

// # # # # # # # # # # # # # # # # # # # #
// #  MoveTo                             #
// # # # # # # # # # # # # # # # # # # # #

static void Camera_CalculateMove(Camera* cam, f32 x, f32 y, f32 z) {
	Vec3f zero = Math_Vec3f_New(0, 0, 0);
	Vec3f upOffset = Math_Vec3f_New(0, 1, 0);
	Vec3f rOffset = Math_Vec3f_New(1, 0, 0);
	Vec3f atOffset = Math_Vec3f_New(0, 0, -cam->dist);
	
	Matrix_Push();
	Matrix_Translate(cam->at.x + x, cam->at.y + y, cam->at.z + z, MTXMODE_NEW);
	
	// cam->vYaw = cam->yaw;
	// cam->vPitch = cam->pitch;
	Matrix_RotateY_s(cam->yaw, MTXMODE_APPLY);
	Matrix_RotateX_s(cam->pitch, MTXMODE_APPLY);
	Matrix_RotateZ_s(cam->roll, MTXMODE_APPLY);
	
	Matrix_Translate(cam->offset.x, cam->offset.y, cam->offset.z, MTXMODE_APPLY);
	cam->offset = Math_Vec3f_New(0, 0, 0);
	Matrix_MultVec3f(&zero, &cam->at);
	
	Matrix_MultVec3f(&upOffset, &cam->up);
	Matrix_MultVec3f(&rOffset, &cam->right);
	
	cam->up = Math_Vec3f_Sub(cam->up, cam->at);
	cam->right = Math_Vec3f_Sub(cam->right, cam->at);
	Matrix_MultVec3f(&atOffset, &cam->eye);
	
	Matrix_Pop();
}

static void Camera_UpdateMoveTo(View3D* this, Input* input) {
	Camera* cam = this->currentCamera;
	
	if (this->moveToTarget && input->mouse.clickL.hold == false) {
		Vec3f p = this->targetPos;
		
		f32 x = Math_DelSmoothStepToF(&this->targetPos.x, 0, 0.25f, this->targetStep, 0.001f);
		f32 z = Math_DelSmoothStepToF(&this->targetPos.z, 0, 0.25f, this->targetStep, 0.001f);
		f32 y = Math_DelSmoothStepToF(&this->targetPos.y, 0, 0.25f, this->targetStep, 0.001f);
		
		p = Math_Vec3f_Sub(p, this->targetPos);
		
		Camera_CalculateMove(cam, p.x, p.y, p.z);
		
		if (x + z + y < 0.1f)
			this->moveToTarget = false;
	} else
		this->moveToTarget = false;
}

// # # # # # # # # # # # # # # # # # # # #
// #  SwapTo                             #
// # # # # # # # # # # # # # # # # # # # #

static void Camera_UpdateSwapTo(View3D* this, Input* input) {
	Camera* cam = this->currentCamera;
	u8 isSwapping = this->flagSwapTo;
	
	if (this->cameraControl) {
		s32 pass = -1;
		s32 key[] = {
			KEY_KP_1,
			KEY_KP_3,
			KEY_KP_7,
		};
		Vec3f target[] = {
			{ 0,  0,   0   },
			{ 0,  90,  0   },
			{ 90, 0,   0   },
		};
		
		for (s32 i = 0; i < ArrayCount(key); i++) {
			if (Input_GetKey(input, key[i])->press) {
				pass = i;
				break;
			}
		}
		
		if (pass > -1) {
			this->flagSwapTo = true;
			
			for (s32 i = 0; i < 3; i++)
				this->rotSwapTo.axis[i] = DegToBin(target[pass].axis[i]);
		} else {
			if (Input_GetKey(input, KEY_KP_9)->press) {
				// Invert
				Vec3f front;
				Vec3f up;
				f32 fdot;
				
				this->flagSwapTo = true;
				if (isSwapping)
					front = Math_Vec3f_New(SinS(this->rotSwapTo.y), SinS(-this->rotSwapTo.x), CosS(this->rotSwapTo.y));
				
				else
					front = Math_Vec3f_LineSegDir(cam->eye, cam->at);
				
				up = Math_Vec3f_New(0, 1, 0);
				fdot = fabsf(Math_Vec3f_Dot(front, up));
				
				printf_info("Dot: %f", fdot);
				
				if (fdot < 0.9995f) {
					this->rotSwapTo.y += DegToBin(180);
				} else {
					this->rotSwapTo.x += DegToBin(180);
				}
			} else {
				Vec3s r = Math_Vec3s_New(cam->pitch, cam->yaw, cam->roll);
				Vec3s p =  { 0, 0, 0 };
				
				if (Input_GetKey(input, KEY_KP_4)->press || Input_GetKey(input, KEY_KP_6)->press) {
					s32 m = Input_GetKey(input, KEY_KP_4)->press ? -1 : 1;
					
					// Sideways
					this->flagSwapTo = true;
					p.y += DegToBin(15) * m;
				}
				
				if (Input_GetKey(input, KEY_KP_2)->press || Input_GetKey(input, KEY_KP_8)->press) {
					s32 m = Input_GetKey(input, KEY_KP_2)->press ? -1 : 1;
					
					// Sideways
					this->flagSwapTo = true;
					p.x += DegToBin(15) * m;
				}
				
				if (isSwapping)
					p = Math_Vec3s_Add(p, Math_Vec3s_Sub(this->rotSwapTo, r));
				
				this->rotSwapTo = Math_Vec3s_Add(r, p);
			}
		}
		
		if (Input_GetKey(input, KEY_KP_5)->press)
			this->ortho ^= true;
	}
	
	if (this->flagSwapTo && input->mouse.click.hold == false) {
		s16 x = Math_SmoothStepToS(&cam->pitch, this->rotSwapTo.x, 3, DegToBin(45), 1);
		s16 z = Math_SmoothStepToS(&cam->yaw, this->rotSwapTo.y, 3, DegToBin(45), 1);
		s16 y = Math_SmoothStepToS(&cam->roll, this->rotSwapTo.z, 3, DegToBin(45), 1);
		
		Camera_CalculateOrbit(cam);
		
		if (!x && !y && !z)
			this->flagSwapTo = false;
	} else
		this->flagSwapTo = false;
}

// # # # # # # # # # # # # # # # # # # # #
// #                                     #
// # # # # # # # # # # # # # # # # # # # #

void View_Init(View3D* this, Input* inputCtx) {
	Camera* cam;
	
	this->currentCamera = &this->camera[0];
	cam = this->currentCamera;
	
	cam->dist = cam->targetDist = 300.f;
	cam->speedMod = 5.0f;
	
	Camera_CalculateFly(cam);
	Matrix_LookAt(&this->viewMtx, cam->eye, cam->at, cam->up);
	
	this->fovyTarget = this->fovy = 65;
	this->near = 10.0;
	this->far = 12800.0;
	this->scale = 1;
	this->mode = CAM_MODE_ALL;
}

void View_Update(View3D* this, Input* inputCtx, Split* split) {
	void (*camMode[])(View3D*, Input*) = {
		Camera_FlyMode,
		Camera_OrbitMode,
	};
	Camera* cam = this->currentCamera;
	
	this->split = split;
	this->usePreCalcRay = false;
	this->isControlled = false;
	
	if (this->ortho)
		Matrix_Ortho(
			&this->projMtx,
			cam->dist,
			(f32)this->split->rect.w / (f32)this->split->rect.h,
			this->near,
			this->far
		);
	else
		Matrix_Projection(
			&this->projMtx,
			this->fovy,
			(f32)this->split->rect.w / (f32)this->split->rect.h,
			this->near,
			this->far,
			this->scale
		);
	
	if (this->mode == CAM_MODE_ALL)
		for (s32 i = 0; i < ArrayCount(camMode); i++)
			camMode[i](this, inputCtx);
	
	else
		camMode[this->mode](this, inputCtx);
	
	Camera_UpdateMoveTo(this, inputCtx);
	Camera_UpdateSwapTo(this, inputCtx);
	
	Matrix_LookAt(&this->viewMtx, cam->eye, cam->at, cam->up);
	
	Matrix_Scale(1.0, 1.0, 1.0, MTXMODE_NEW);
	Matrix_ToMtxF(&this->modelMtx);
	
	if (this->cameraControl) {
		s32 keyList[] = {
			KEY_W, KEY_A, KEY_S, KEY_D
		};
		
		for (s32 i = 0; i < ArrayCount(keyList); i++) {
			if (Input_GetKey(inputCtx, keyList[i])->hold)
				this->isControlled = true;
		}
	}
}

// # # # # # # # # # # # # # # # # # # # #
// #                                     #
// # # # # # # # # # # # # # # # # # # # #

bool View_CheckControlKeys(Input* input) {
	if (Input_GetMouse(input, MOUSE_L)->press)
		return true;
	if (Input_GetMouse(input, MOUSE_M)->press)
		return true;
	if (Input_GetKey(input, KEY_W)->press)
		return true;
	if (Input_GetKey(input, KEY_A)->press)
		return true;
	if (Input_GetKey(input, KEY_S)->press)
		return true;
	if (Input_GetKey(input, KEY_D)->press)
		return true;
	for (s32 i = KEY_KP_0; i <= KEY_KP_9; i++)
		if (Input_GetKey(input, i)->press)
			return true;
	if (input->mouse.scrollY)
		return true;
	
	return false;
}

RayLine View_GetPointRayLine(View3D* this, Vec2f point) {
	MtxF modelView;
	Rect dispRect = this->split->dispRect;
	Vec3f projA = Math_Vec3f_New(point.x, point.y, 0.0f);
	Vec3f projB = Math_Vec3f_New(point.x, point.y, 1.0f);
	Vec3f rayA, rayB;
	
	Matrix_MtxFMtxFMult(&this->modelMtx, &this->viewMtx, &modelView);
	Matrix_Unproject(&modelView, &this->projMtx, &projA, &rayA, dispRect.w, dispRect.h);
	Matrix_Unproject(&modelView, &this->projMtx, &projB, &rayB, dispRect.w, dispRect.h);
	
	return RayLine_New(rayA, rayB);
}

RayLine View_GetCursorRayLine(View3D* this) {
	if (this->usePreCalcRay)
		return this->ray;
	
	this->usePreCalcRay = true;
	
	return this->ray = View_GetPointRayLine(this, Math_Vec2f_New(UnfoldVec2(this->split->mousePos)));
}

void View_MoveTo(View3D* this, Vec3f pos) {
	this->targetPos = Math_Vec3f_Sub(pos, this->currentCamera->at);
	this->moveToTarget = true;
	this->targetStep = Math_Vec3f_DistXYZ(pos, this->currentCamera->at) * 0.25f;
}

Vec3f View_OrientDirToView(View3D* this, Vec3f dir) {
	Vec3f pos;
	
	Matrix_Push();
	Matrix_RotateY_s(this->currentCamera->yaw, MTXMODE_APPLY);
	Matrix_RotateX_s(this->currentCamera->pitch, MTXMODE_APPLY);
	Matrix_RotateZ_s(this->currentCamera->roll, MTXMODE_APPLY);
	Matrix_MultVec3f(&dir, &pos);
	Matrix_Pop();
	
	return pos;
}

Vec2f View_GetScreenPos(View3D* this, Vec3f point) {
	MtxF modelView;
	f32 w = this->split->dispRect.w * 0.5f;
	f32 h = this->split->dispRect.h * 0.5f;
	Vec4f pos;
	
	Matrix_MtxFMtxFMult(&this->projMtx, &this->viewMtx, &modelView);
	Matrix_MultVec3fToVec4f_Ext(&point, &pos, &modelView);
	
	return Math_Vec2f_New(
		w + (pos.x / pos.w) * w,
		h - (pos.y / pos.w) * h
	);
}

f32 View_DepthFactor(View3D* this, Vec3f point) {
	MtxF modelview;
	Vec4f sp;
	
	Matrix_MtxFMtxFMult(&this->projMtx, &this->viewMtx, &modelview);
	Matrix_MultVec3fToVec4f_Ext(&point, &sp, &modelview);
	
	return sp.w;
}

f32 View_DimFactor(View3D* this) {
	return sqrtf(this->split->rect.w * this->split->rect.h);
}