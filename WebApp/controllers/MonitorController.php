<?php

namespace app\controllers;

use Yii;
use app\models\camera;
use app\models\image;
use app\models\search\CameraSearch;
use yii\web\Controller;
use yii\web\NotFoundHttpException;
use yii\filters\VerbFilter;

class MonitorController extends \yii\web\Controller
{
    public function actionIndex()
    {
        $a = new image();
        return $this->render('index');
    }

    public function actionDetail($cam_id,$date="")
    {
        //$camera = Camera::findOne($cam_id);
        //$cam_images = $camera->getImages()->all();
        //$model = $this->findModel($cam_id);
        //$cam_images = Image::find()->where(["cam_id"=>$cam_id])->all();
        $cam_images = $this->findAllImages($cam_id,$date);
        return $this->render('detail',[
            //'camera'=>$camera,
            'cam_images'=>$cam_images
        ]);
    }

    public function actionHistory()
    {
        return $this->render('history');
    }

    public function actionOnline()
    {
        return $this->render('online');
    }

    /**
     * Finds the image model based on its primary key value.
     * If the model is not found, a 404 HTTP exception will be thrown.
     * @param integer $id
     * @return image the loaded model
     * @throws NotFoundHttpException if the model cannot be found
     */
    protected function findModel($id)
    {
        if (($model = image::findOne($id)) !== null)
        {
            return $model;
        }
        else
        {
            throw new NotFoundHttpException('The requested page does not exist.');
        }
    }

    /***
     * 查找某一个相机下的所有图像
     * @param $cam_id
     */
    private function findAllImages($cam_id,$date="")
    {
        $table_name = Image::tableName($date);
        $all_images = Image::findBySql("
        SELECT * FROM {$table_name}
        WHERE `cam_id`={$cam_id}
        ")->all();

        //$all_images = Image::find()->joinWith(['camera'])->where(['cam_id' => $cam_id])->all();
        return $all_images;
    }

}
