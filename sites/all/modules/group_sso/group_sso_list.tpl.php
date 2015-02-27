
<div class="content_list">
	<div class="tool_list">
		<a href="group-sso/new">Nuevo grupo</a>
		<a href="group-sso/new">Asignar grupo</a>
	</div>
	<div class="rows">
		<table>
			<tr>
				<th></th>
				<th></th>
				<th>Nombre de grupo</th>
				<th>Escuela</th>
				<th>Nivel</th>
				<th>Grado</th>
			</tr>
			<?php 
			function nivel($string){
				switch ($string) {
					case '1':
						echo 'Primaria';
						break;
					case '2':
						echo 'Secundaria';
						break;
					case '3':
						echo 'Bachillerato';
						break;	
				}
			}
			?>
			<?php $i=0; foreach($grupos as $g):?>
				<tr>
					<td class="fila_<?php echo $i%2?>"><a href="group-sso/edit/<?php echo $g->gid?>">Edit</button></td>
					<td class="fila_<?php echo $i%2?>"><a href="<?php echo $g->gid?>" class="del">del</button></td>
					<td class="fila_<?php echo $i%2?>"><?php echo $g->groups?></td>
					<td class="fila_<?php echo $i%2?>"><?php echo $escuela[0]->name;?></td>
					<td class="fila_<?php echo $i%2?>"><?php echo nivel($g->level)?></td>
					<td class="fila_<?php echo $i%2?>"><?php echo $g->grade?></td>
				</tr>
			<?php $i++; endforeach;?>
		</table>
	</div>
</div>
<script src="http://code.jquery.com/jquery-1.11.1.min.js"></script>
<script src="http://code.jquery.com/jquery-migrate-1.2.1.min.js"></script>
<script>
	$(function(){
		$('tr').hover(function(){
			$(this).children($('td')).addClass('hover');
		},function(){
			$(this).children($('td')).removeClass('hover');
		});
		
		$('.del').click(function(e){
			e.preventDefault();
			$.ajax({
				url:'group-sso/del/'+$(this).attr('href'),
				success:function(dataR){
					console.log(dataR);
				}
			});
			
		})
	});
</script>
<style>

.fila_1 {
    background-color: #ccc;
}


.hover{
    background-color: #aaaaaa;
    cursor:pointer;
}
</style>

