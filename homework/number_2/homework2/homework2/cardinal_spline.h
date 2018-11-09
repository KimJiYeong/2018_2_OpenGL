#pragma once

float Cardinal_Spline(int start , int ctrl_pt, int arrive, double out_put,int t) {
	out_put =
		(
		(((2 * (t*t)) - (3 * t) + 1)*start) +
			(((-4 * (t* t)) + (4 * t))*ctrl_pt) +
			((2 * (t*t) - t) * arrive))
		;

	if (t == 1) {
		return;
	}
	else {
		Cardinal_Spline(start, ctrl_pt, arrive, out_put, t + 0.2);
		return out_put;
	}
}

/*
click_pt[i].spline[j].x =
(
((-(click_pt[i % 10].t*click_pt[(i) % 10].t *click_pt[(i) % 10].t)
+ (2 * (click_pt[(i) % 10].t * click_pt[(i) % 10].t)) -
click_pt[(i) % 10].t) *click_pt[(i) % 10].pos.x) +

((3 * (click_pt[(i) % 10].t* click_pt[(i) % 10].t *click_pt[(i) % 10].t) -
(5 * (click_pt[(i) % 10].t * click_pt[(i) % 10].t)) + 2) *click_pt[(i + 1) % 10].pos.x) +

((-3 * (click_pt[(i) % 10].t*click_pt[(i) % 10].t *click_pt[(i) % 10].t) +
(4 * (click_pt[(i) % 10].t * click_pt[(i) % 10].t)) +
click_pt[(i) % 10].t) *click_pt[(i + 2) % 10].pos.x)+

((((click_pt[(i) % 10].t*click_pt[(i) % 10].t *click_pt[(i) % 10].t) -
(click_pt[(i) % 10].t*click_pt[(i) % 10].t)) * click_pt[(i + 3) % 10].pos.x))
) / 2;
click_pt[i].spline[j].z =
(
((-(click_pt[i].t*click_pt[(i) % 10].t *click_pt[(i) % 10].t) + (2 * (click_pt[(i) % 10].t * click_pt[(i) % 10].t)) - click_pt[(i) % 10].t) *click_pt[(i) % 10].pos.z) +
((3 * (click_pt[(i) % 10].t*click_pt[(i) % 10].t *click_pt[(i) % 10].t) - (5 * (click_pt[(i) % 10].t * click_pt[(i) % 10].t)) + 2) *click_pt[(i + 1) % 10].pos.z) +
((-3 * (click_pt[(i) % 10].t*click_pt[(i) % 10].t *click_pt[(i) % 10].t) + (4 * (click_pt[(i) % 10].t * click_pt[(i) % 10].t)) + click_pt[(i) % 10].t) *click_pt[(i + 2) % 10].pos.z) +
((((click_pt[(i) % 10].t*click_pt[(i) % 10].t *click_pt[(i) % 10].t) - (click_pt[(i) % 10].t*click_pt[(i) % 10].t)) * click_pt[(i + 3) % 10].pos.z))
)/2;
*/