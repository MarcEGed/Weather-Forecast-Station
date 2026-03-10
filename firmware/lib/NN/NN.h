#ifndef NN_H
#define NN_H

extern float nn_W1[7][10];
extern float nn_B1[10];
extern float nn_W2[10];
extern float nn_B2;

extern float delta_mean;
extern float delta_std;
extern float y_mean;
extern float y_std;

float normalize_day(int day);
float normalize_hour(int hour);

float predict_NextHour(float temps[6], float hour_norm, float day_norm);

#endif