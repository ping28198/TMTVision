<?php

namespace app\models;

use Yii;

/**
 * This is the model class for table "camera".
 *
 * @property integer $id
 * @property integer $cam_num
 * @property string $cam_name
 * @property string $cam_folder
 * @property string $server_ip
 * @property string $location_lat
 * @property string $location_lng
 * @property string $warning_level
 * @property string $cam_status
 * @property string $algo_mask
 * @property string $algo_status
 */
class Camera extends \yii\db\ActiveRecord
{
    /**
     * @inheritdoc
     */
    public static function tableName()
    {
        return 'camera';
    }

    /**
     * @inheritdoc
     */
    public function rules()
    {
        return [
            [['cam_num'], 'integer'],
            [['cam_name', 'cam_folder', 'server_ip', 'location_lat', 'location_lng', 'warning_level', 'cam_status', 'algo_mask', 'algo_status'], 'string', 'max' => 255]
        ];
    }

    /**
     * @inheritdoc
     */
    public function attributeLabels()
    {
        return [
            'id' => 'ID',
            'cam_num' => 'Cam Num',
            'cam_name' => 'Cam Name',
            'cam_folder' => '图像目录',
            'server_ip' => 'Server Ip',
            'location_lat' => 'Location Lat',
            'location_lng' => 'Location Lng',
            'warning_level' => 'Warning Level',
            'cam_status' => 'Cam Status',
            'algo_mask' => 'Algo Mask',
            'algo_status' => 'Algo Status',
        ];
    }
}
