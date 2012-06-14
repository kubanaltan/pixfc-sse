#include <stdio.h>

int main(int argc, char **argv) {

	// RGB downsample
	check_nnb_422_downsample_ag_rb_vectors();
	check_nnb_422_downsample_r_g_b_vectors();
	check_avg_422_downsample_ag_rb_vectors_n_save_previous();

	return 0;
}
