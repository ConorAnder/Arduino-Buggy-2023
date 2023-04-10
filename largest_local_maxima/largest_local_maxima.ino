#include <vector>

int max1, max2;

std::vector<float> angles, distances, a;

void setup() {
  Serial.begin(9600);
  max1 = 0;
  max2 = 0;
}

void loop() {
  a.clear();

  //Serial.println(a[3]);
  float b[] = { 1.1, 2.1, 3.1, 3.1, 3.1, 2.1, 3.1, 4.1, 5.1, 4.1, 3.1, 2.1, 1.1 };
  for (int i = 0; i < sizeof(b) / sizeof(b[0]); i++) {
    a.push_back(b[i]);
  }

  getPeaks(a, max1, max2);
  Serial.println(String(a[max1]) + " " + String(a[max2]));
}

void getPeaks(std::vector<float> distances, int& max1, int& max2) {
  float max = 0;
  bool ascending = true;
  for (int i = 1; i < distances.size() - 1; i++) {
    if (distances[i] > distances[i-1])
      ascending = true;

    else if (distances[i] < distances[i-1])
      ascending = false;

    if (ascending && distances[i + 1] < distances[i]) {
      if (distances[i] > max) {
        max2 = max1;
        max1 = i;
        max = distances[i];
      }
    }
  }
}
