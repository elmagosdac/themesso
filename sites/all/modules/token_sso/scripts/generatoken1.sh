#!/usr/bin/env php
<?
 /**@brief _generaToken
  * 
  *Funcion que realiza la generacion de los tokens
  * 
  * @return $token es el resultado final
  * 
 * @author Miguel Angel Chávez Obregón
 * @date 2014-09-20
  */
function _generaToken(){
echo "Genera Token\n";
  $an = '0123456789abcdefghijkmnpqrstuvwxyz';
  $su = strlen($an)-1;
  return substr($an,rand(0,$su),1).
         substr($an,rand(0,$su),1).
         substr($an,rand(0,$su),1).
         substr($an,rand(0,$su),1).
         substr($an,rand(0,$su),1).
        substr($an,rand(0,$su),1);
}

 /**@brief _almacenaToken
  * 
  *Funcion Mendiante la cual se realiza el almacenamiento del los tokens generados
  *Los valores a porcesar se recuperan del un archivo csv llamado uniqs, donde los 
  * registros fueron ordenas y se eliminan los duplicados para meter registros unicos.
  * 
  * @return $total es el total de tokens ingresados en caso de 0 se insertaron todos; 
  * diferente de 0 son tokens que estaban duplicados y hay que generar de nuevo
  * 
 * @author Miguel Angel Chávez Obregón
 * @date 2014-09-20
  */
function _almacenaToken(){
echo "Entra Almacenamiento\n";
$total = 0;

 $registros = file('/var/www/dev/sso/sites/all/modules/token_sso/scripts/uniqs.csv',FILE_SKIP_EMPTY_LINES);
 foreach($registros as $row){
  $data = explode(",",$row);
  $inserta =_insertaDB($data);
  $total =$total+$inserta;
 }
return $total;
}

 /**@brief _conectDB
  * 
  *Funcion Mendiante se realiza la conexion a la base de datos
  * 
 * @author Miguel Angel Chávez Obregón
 * @date 2014-10-28
  */
function _conectDB(){
  $db_host = 'localhost';
  $db_user = 'desarrollo';
  $db_pass = 'mysql';
  $db_base = 'sso';
  $link = mysql_connect($db_host,$db_user,$db_pass) or die('Fallo conexio: '.mysql_error());
  $db =  mysql_select_db($db_base,$link) or die('fallo conexion a la base: '.mysql_error());
}

 /**@brief _closeDB
  * 
  *Funcion Mendiante se realiza el cierre de la base de datos
  * 
 * @author Miguel Angel Chávez Obregón
 * @date 2014-10-28
  */
function _closeDB(){
 mysql_close();

}

 /**@brief _insertaDB
  * 
  *Funcion Mendiante se realiza la insercion a la base de datos de los tokens generados
  * Se valida que no esten duplicados en la base de datos, en caso de estarlo solicitan
  * los registros faltantes para su incersion
  *
  *@param $token es una arreglo con los parametros necesarios para realizar el registro
  * de los tokens
  *@return $inserta total de registros insertados.
  *  
 * @author Miguel Angel Chávez Obregón
 * @date 2014-10-28
  */
function _insertaDB($token){
 echo "entra a inserta\n ";
$inserta = 0;


  $result = mysql_query("select count(token) as duplicado from sso_token where token='".$token[0]."'");

  if(mysql_result($result,0)==0){
   $qry = "insert into sso_token(token, status, isbn, id_product, id_application, application_url, permanent, created)  values('".$token[0]."','".$token[1]."','".$token[2]."','".$token[3]."','".$token[4]."','".$token[5]."','".$token[6]."','".date('YYmd')."')";
   $result = mysql_query($qry);
  	 $inserta++;
  }
 return $inserta;
}

 /**@brief _recoverTask
  * 
  *Funcion Mendiante se recupera los datos del los tokens a ser realizados

  *@return arreglo asociativo de los tokens a procesar
  *  
 * @author Miguel Angel Chávez Obregón
 * @date 2014-10-28
  */
  
function _recoverTask(){
 echo "Entra recuepera\n"; 
 
 $result = mysql_query("select * from sso_procesa_token where status='0'");
 return mysql_fetch_assoc($result);
}

 /**@brief _cambiaStatus()
  * 
  *Funcion Mendiante se cambia el estatus del registro procesado al 100%
  *  
  * @author Miguel Angel Chávez Obregón
  * @date 2014-10-28
*/
function _cambiaStatus($id){
	
  $query = "update sso_procesa_token set status = 3 where id_sso_procesa_token='".$id."'";
   mysql_query($query);  
	
}
 /**@brief _cleanFiles
  * 
  *Funcion Mendiante la cual se eliminan los registros anteriores para iniciar 
  * desde 0 los nuevos registros
  *  
 * @author Miguel Angel Chávez Obregón
 * @date 2014-10-28
  */
function _cleanFiles(){

 if(file_exists('/var/www/dev/sso/sites/all/modules/token_sso/scripts/news.csv')){
  unlink('/var/www/dev/sso/sites/all/modules/token_sso/scripts/news.csv');
 }

 if(file_exists('/var/www/dev/sso/sites/all/modules/token_sso/scripts/uniqs.csv')){
  unlink('/var/www/dev/sso/sites/all/modules/token_sso/scripts/uniqs.csv');
 }
}

 /**@brief _createFiles
  * 
  *Funcion Mendiante la cual se general los archivos csv con los tokens a inserta
  * el archivo news.csv son los tokens generados
  * el archivo uniqs.csv son los tokens depurados donde ya no existen valores duplicados.
  * 
 * @author Miguel Angel Chávez Obregón
 * @date 2014-10-28
  */
function _createFiles($num,$isbn,$idInterno,$permanent,$id_application){
echo "Crea Files\n";
//exec('cd /var/www/dev/sso/sites/all/modules/token_sso/scripts');
 $fh = fopen('/var/www/dev/sso/sites/all/modules/token_sso/scripts/news.csv','a+');

 for($i=1;$i<=$num;$i++){
	  $token= _generaToken();
	  $url = "http://sso.smit.com.mx/dev/sso/qr/$token";
	  $data= array(
	    'token'=>$token,
	    'status'=>0,
	    'isbn'=>$isbn,
	    'id_product'=>$idInterno,
	    'id_application'=>$id_application,
	    'application_url'=>$url,
	    'permanent'=>$permanent,
	  );
	  fputcsv($fh,$data);
	}
	fclose($fh);
	exec('/bin/cat /var/www/dev/sso/sites/all/modules/token_sso/scripts/news.csv | sort | uniq >> /var/www/dev/sso/sites/all/modules/token_sso/scripts/uniqs.csv');
	exec('/bin/chmod +x /var/www/dev/sso/sites/all/modules/token_sso/scripts/uniqs.csv');
}

	_conectDB();
	_cleanFiles();
	
	$data = _recoverTask();
	$id= $data['id_sso_procesa_token'];
	$num = $data['amount'];
	$isbn = $data['isbn'];
	$idInterno = $data['idInterno'];
	$permanent = $data['permanent'];
	$id_application = $data['id_application'];
	$procesadas =0;
	$total = $num;
	
	do{
	
	  echo "Numero: ".$num." -- Fin: ".$procesadas."\n";
	 _createFiles($num,$isbn,$idInterno,$permanent,$id_application);
	
	   $procesadas = _almacenaToken();
	
	  if($num > $procesadas){
	  	$num = $num -$procesadas;
	  }else{
	  	$num = 0;
	  }
	
	}while($num > 0);
	
	echo "Total insertados ".$total."\n";
	_cambiaStatus($id);
	_closeDB();
?>
