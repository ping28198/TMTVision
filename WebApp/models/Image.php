<?php

namespace app\models;

use Yii;

/**
 * This is the model class for table "image_20160416_1".
 *
 * @property integer $id
 * @property string $image_path
 * @property string $grab_time
 * @property integer $defects_num
 * @property string $defects_pos
 * @property double $height
 * @property double $width
 */
class Image extends \yii\db\ActiveRecord
{
    /**
     * @inheritdoc
     */
    public static function tableName($suffix="")
    {
        return 'image'.$suffix;
    }

    /**
     * @inheritdoc
     */
    public function rules()
    {
        return [
            [['grab_time'], 'safe'],
            [['defects_num'], 'integer'],
            [['height', 'width'], 'number'],
            [['image_path', 'defects_pos'], 'string', 'max' => 255]
        ];
    }

    /**
     * @inheritdoc
     */
    public function attributeLabels()
    {
        return [
            'id' => 'ID',
            'image_path' => 'Image Path',
            'grab_time' => 'Grab Time',
            'defects_num' => 'Defects Num',
            'defects_pos' => 'Defects Pos',
            'height' => 'Height',
            'width' => 'Width',
        ];
    }


}
