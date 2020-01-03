#pragma once
// MESSAGE OPENHD_CAMERA_STATUS PACKING

#define MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS 1200

MAVPACKED(
typedef struct __mavlink_openhd_camera_status_t {
 uint8_t brightness; /*<  brightness*/
 uint8_t contrast; /*<  contrast*/
 uint8_t saturation; /*<  saturation*/
 uint8_t target_system; /*<  system id of the requesting system*/
 uint8_t target_component; /*<  component id of the requesting component*/
}) mavlink_openhd_camera_status_t;

#define MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_LEN 5
#define MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_MIN_LEN 5
#define MAVLINK_MSG_ID_1200_LEN 5
#define MAVLINK_MSG_ID_1200_MIN_LEN 5

#define MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_CRC 97
#define MAVLINK_MSG_ID_1200_CRC 97



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_OPENHD_CAMERA_STATUS { \
    1200, \
    "OPENHD_CAMERA_STATUS", \
    5, \
    {  { "brightness", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_openhd_camera_status_t, brightness) }, \
         { "contrast", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_openhd_camera_status_t, contrast) }, \
         { "saturation", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_openhd_camera_status_t, saturation) }, \
         { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 3, offsetof(mavlink_openhd_camera_status_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_openhd_camera_status_t, target_component) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_OPENHD_CAMERA_STATUS { \
    "OPENHD_CAMERA_STATUS", \
    5, \
    {  { "brightness", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_openhd_camera_status_t, brightness) }, \
         { "contrast", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_openhd_camera_status_t, contrast) }, \
         { "saturation", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_openhd_camera_status_t, saturation) }, \
         { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 3, offsetof(mavlink_openhd_camera_status_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_openhd_camera_status_t, target_component) }, \
         } \
}
#endif

/**
 * @brief Pack a openhd_camera_status message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param brightness  brightness
 * @param contrast  contrast
 * @param saturation  saturation
 * @param target_system  system id of the requesting system
 * @param target_component  component id of the requesting component
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_openhd_camera_status_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t brightness, uint8_t contrast, uint8_t saturation, uint8_t target_system, uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_LEN];
    _mav_put_uint8_t(buf, 0, brightness);
    _mav_put_uint8_t(buf, 1, contrast);
    _mav_put_uint8_t(buf, 2, saturation);
    _mav_put_uint8_t(buf, 3, target_system);
    _mav_put_uint8_t(buf, 4, target_component);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_LEN);
#else
    mavlink_openhd_camera_status_t packet;
    packet.brightness = brightness;
    packet.contrast = contrast;
    packet.saturation = saturation;
    packet.target_system = target_system;
    packet.target_component = target_component;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_MIN_LEN, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_LEN, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_CRC);
}

/**
 * @brief Pack a openhd_camera_status message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param brightness  brightness
 * @param contrast  contrast
 * @param saturation  saturation
 * @param target_system  system id of the requesting system
 * @param target_component  component id of the requesting component
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_openhd_camera_status_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t brightness,uint8_t contrast,uint8_t saturation,uint8_t target_system,uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_LEN];
    _mav_put_uint8_t(buf, 0, brightness);
    _mav_put_uint8_t(buf, 1, contrast);
    _mav_put_uint8_t(buf, 2, saturation);
    _mav_put_uint8_t(buf, 3, target_system);
    _mav_put_uint8_t(buf, 4, target_component);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_LEN);
#else
    mavlink_openhd_camera_status_t packet;
    packet.brightness = brightness;
    packet.contrast = contrast;
    packet.saturation = saturation;
    packet.target_system = target_system;
    packet.target_component = target_component;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_MIN_LEN, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_LEN, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_CRC);
}

/**
 * @brief Encode a openhd_camera_status struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param openhd_camera_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_openhd_camera_status_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_openhd_camera_status_t* openhd_camera_status)
{
    return mavlink_msg_openhd_camera_status_pack(system_id, component_id, msg, openhd_camera_status->brightness, openhd_camera_status->contrast, openhd_camera_status->saturation, openhd_camera_status->target_system, openhd_camera_status->target_component);
}

/**
 * @brief Encode a openhd_camera_status struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param openhd_camera_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_openhd_camera_status_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_openhd_camera_status_t* openhd_camera_status)
{
    return mavlink_msg_openhd_camera_status_pack_chan(system_id, component_id, chan, msg, openhd_camera_status->brightness, openhd_camera_status->contrast, openhd_camera_status->saturation, openhd_camera_status->target_system, openhd_camera_status->target_component);
}

/**
 * @brief Send a openhd_camera_status message
 * @param chan MAVLink channel to send the message
 *
 * @param brightness  brightness
 * @param contrast  contrast
 * @param saturation  saturation
 * @param target_system  system id of the requesting system
 * @param target_component  component id of the requesting component
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_openhd_camera_status_send(mavlink_channel_t chan, uint8_t brightness, uint8_t contrast, uint8_t saturation, uint8_t target_system, uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_LEN];
    _mav_put_uint8_t(buf, 0, brightness);
    _mav_put_uint8_t(buf, 1, contrast);
    _mav_put_uint8_t(buf, 2, saturation);
    _mav_put_uint8_t(buf, 3, target_system);
    _mav_put_uint8_t(buf, 4, target_component);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS, buf, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_MIN_LEN, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_LEN, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_CRC);
#else
    mavlink_openhd_camera_status_t packet;
    packet.brightness = brightness;
    packet.contrast = contrast;
    packet.saturation = saturation;
    packet.target_system = target_system;
    packet.target_component = target_component;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS, (const char *)&packet, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_MIN_LEN, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_LEN, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_CRC);
#endif
}

/**
 * @brief Send a openhd_camera_status message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_openhd_camera_status_send_struct(mavlink_channel_t chan, const mavlink_openhd_camera_status_t* openhd_camera_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_openhd_camera_status_send(chan, openhd_camera_status->brightness, openhd_camera_status->contrast, openhd_camera_status->saturation, openhd_camera_status->target_system, openhd_camera_status->target_component);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS, (const char *)openhd_camera_status, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_MIN_LEN, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_LEN, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_CRC);
#endif
}

#if MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_openhd_camera_status_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t brightness, uint8_t contrast, uint8_t saturation, uint8_t target_system, uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, brightness);
    _mav_put_uint8_t(buf, 1, contrast);
    _mav_put_uint8_t(buf, 2, saturation);
    _mav_put_uint8_t(buf, 3, target_system);
    _mav_put_uint8_t(buf, 4, target_component);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS, buf, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_MIN_LEN, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_LEN, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_CRC);
#else
    mavlink_openhd_camera_status_t *packet = (mavlink_openhd_camera_status_t *)msgbuf;
    packet->brightness = brightness;
    packet->contrast = contrast;
    packet->saturation = saturation;
    packet->target_system = target_system;
    packet->target_component = target_component;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS, (const char *)packet, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_MIN_LEN, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_LEN, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_CRC);
#endif
}
#endif

#endif

// MESSAGE OPENHD_CAMERA_STATUS UNPACKING


/**
 * @brief Get field brightness from openhd_camera_status message
 *
 * @return  brightness
 */
static inline uint8_t mavlink_msg_openhd_camera_status_get_brightness(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field contrast from openhd_camera_status message
 *
 * @return  contrast
 */
static inline uint8_t mavlink_msg_openhd_camera_status_get_contrast(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  1);
}

/**
 * @brief Get field saturation from openhd_camera_status message
 *
 * @return  saturation
 */
static inline uint8_t mavlink_msg_openhd_camera_status_get_saturation(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  2);
}

/**
 * @brief Get field target_system from openhd_camera_status message
 *
 * @return  system id of the requesting system
 */
static inline uint8_t mavlink_msg_openhd_camera_status_get_target_system(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  3);
}

/**
 * @brief Get field target_component from openhd_camera_status message
 *
 * @return  component id of the requesting component
 */
static inline uint8_t mavlink_msg_openhd_camera_status_get_target_component(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  4);
}

/**
 * @brief Decode a openhd_camera_status message into a struct
 *
 * @param msg The message to decode
 * @param openhd_camera_status C-struct to decode the message contents into
 */
static inline void mavlink_msg_openhd_camera_status_decode(const mavlink_message_t* msg, mavlink_openhd_camera_status_t* openhd_camera_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    openhd_camera_status->brightness = mavlink_msg_openhd_camera_status_get_brightness(msg);
    openhd_camera_status->contrast = mavlink_msg_openhd_camera_status_get_contrast(msg);
    openhd_camera_status->saturation = mavlink_msg_openhd_camera_status_get_saturation(msg);
    openhd_camera_status->target_system = mavlink_msg_openhd_camera_status_get_target_system(msg);
    openhd_camera_status->target_component = mavlink_msg_openhd_camera_status_get_target_component(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_LEN? msg->len : MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_LEN;
        memset(openhd_camera_status, 0, MAVLINK_MSG_ID_OPENHD_CAMERA_STATUS_LEN);
    memcpy(openhd_camera_status, _MAV_PAYLOAD(msg), len);
#endif
}
