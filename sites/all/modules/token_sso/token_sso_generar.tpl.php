<?php

/** @brief Template token_sso_generar
 * Template para mejorar el desarrollo y administracion de contenidos html
 * @param $form contiene los campos para renderizar en el template
 * @author Sajit David Avila Correa
 * @date 2014-10-13
 */
 ?>
<div class="cont_token_form">
	<form action="<?php echo $form['#action'];?>" method="<?php echo $form['#method'];?>" id="<?php echo $form['#id'];?>">
	<div class="left_col">
		<div class="campos">
			<?php print drupal_render($form['idInterno'])?>
		</div>
		<div class="campos">
			<?php print drupal_render($form['cantidad'])?>
		</div>
		<div class="campos">
			<?php print drupal_render($form['permanent'])?>
		</div>
		<div class="campos">
			<?php print drupal_render($form['application'])?>
		</div>
	</div>
	<div class="right_col">
		<div class="respuesta_ecat">
			<?php print drupal_render($form['ecat'])?>
		</div>
	</div>
	<div class="">
		<?php print drupal_render($form['form_build_id'])?>
		<?php print drupal_render($form['form_token'])?>
		<?php print drupal_render($form['form_id'])?>
	</div>
	<div class="buton_content">
		<?php print drupal_render($form['submit'])?>
	</div>
</div>
<style>
.cont_token_form {
    float: left;
    width: 100%;
}

.left_col {
    float: left;
    width: 60%;
}

.right_col {
	position:relative;
    float: left;
    width: 40%;
}
.buton_content {
    float: left;
    text-align: right;
    width: 100%;
}
#resutado_e_cat {
    margin: 0 auto;
    text-align: left;
    width: 80%;
}

#resutado_e_cat img {
    border-radius: 10px;
    box-shadow: 0 0 5px -2px #1b1b1b;
    width: 80%;
}

.cont_token_form input[type=text] {
	width: 90%;
}


.buton_content .form-submit {
    border-radius: 10px;
    float: right;
    font-size: 1em;
    margin-right: 160px;
}

</style>