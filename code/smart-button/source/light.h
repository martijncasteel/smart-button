/**
 * @file lights.cpp
 * @date 2022-04-05
 * 
 * @brief Interface class for lights.
 */
class Light {
  public:

    // interface for toggling the light
    virtual bool toggle() = 0;
    virtual bool set(int brightness) = 0;

    bool state = false;
    int brightness; // 3-100
};