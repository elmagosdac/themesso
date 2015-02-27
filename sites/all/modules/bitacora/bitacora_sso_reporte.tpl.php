<?php
global $base_url
?>

<!-- DataTables CSS -->
<link rel="stylesheet" type="text/css" href="http://cdn.datatables.net/1.10.4/css/jquery.dataTables.css">

<!-- jQuery -->
<script type="text/javascript" charset="utf8" src="//code.jquery.com/jquery-1.11.2.min.js"></script>
<script src="<?php echo $base_url?>/sites/all/modules/user_sso/js/jquery-migrate-1.2.1.min.js"></script>

<!-- DataTables -->
<script type="text/javascript" charset="utf8" src="http://cdn.datatables.net/1.10.4/js/jquery.dataTables.js"></script>

<script type="text/javascript" charset="utf8">


	$(document).ready( function () {
	    $('#bitacora').DataTable(
		{
			"jQueryUI": true,
			"info": false,
			"processing": true,
			columns:[
				{data:'Nombre'},
				{data:'Correo'},
				{data:'Rol'},
				{data:'Accion'},
				{data:'Operacion'},
	
			]
		 }
	    );    
} );
	
</script>

<?php 
	$html ='';
	foreach ($data as $row) {
		if(!empty($row['nombre'])){
			$html.="<tr>";
			$html .="<td data-search='".$row['nombre']."' data-order='".$row['nombre']."'>".$row['nombre']."</td>";
			$html .="<td>".$row['mail']."</td>";
			$html .="<td data-order='".$row['rol']."'>".$row['rol']."</td>";
			$html .="<td data-order='".$row['action']."'>".$row['action']."</td>";
			$html .="<td>".$row['operation']."</td>";
			$html.="</tr>";
		}
	}
?>
<table id="bitacora"  class="display">
	<thead>
		<tr>
			<th>Nombre</th>
			<th>Correo</th>
			<th>Rol</th>
			<th>Accion</th>
			<th>Operacion</th>
		</tr>
	</thead>
	<tbody>
		<?php echo $html; ?>
	</tbody>
</table>



