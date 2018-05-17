/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bal-khan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/11 05:58:23 by bal-khan          #+#    #+#             */
/*   Updated: 2016/03/11 05:58:28 by bal-khan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "reader.h"

void            reader(t_header *fh, int fd_img)
{
    read(fd_img, &(fh->file_marker1), sizeof(unsigned char));
    read(fd_img, &(fh->file_marker2), sizeof(unsigned char));
    read(fd_img, &(fh->bf_size), sizeof(unsigned int));
    read(fd_img, &(fh->unused1), sizeof(short));
    read(fd_img, &(fh->unused2), sizeof(short));
    read(fd_img, &(fh->image_data_offset), sizeof(unsigned int));
    read(fd_img, &(fh->bi_size), sizeof(unsigned int));
    read(fd_img, &(fh->width), sizeof(int));
    read(fd_img, &(fh->height), sizeof(int));
    read(fd_img, &(fh->planes), sizeof(short));
    read(fd_img, &(fh->bit_pix), sizeof(short));
    read(fd_img, &(fh->bi_compression), sizeof(unsigned int));
    read(fd_img, &(fh->bi_size_image), sizeof(unsigned int));
    read(fd_img, &(fh->bi_xpels_per_meter), sizeof(int));
    read(fd_img, &(fh->bi_ypels_per_meter), sizeof(int));
    read(fd_img, &(fh->bi_clrused), sizeof(unsigned int));
    read(fd_img, &(fh->bi_clr_important), sizeof(unsigned int));
}

void            write_header(t_header *fh)
{
    int fd_img;

    (void)fh;
    fd_img = open("./my_micky.bmp", O_RDWR | O_CREAT);
    write(fd_img, "B", sizeof(unsigned char));
    write(fd_img, "M", sizeof(unsigned char));
    write(fd_img, "3066138", sizeof(unsigned int));
    write(fd_img, "0", sizeof(short));
    write(fd_img, "0", sizeof(short));
    write(fd_img, "138", sizeof(unsigned int));
    write(fd_img, "124", sizeof(unsigned int));
    write(fd_img, "1000", sizeof(int));
    write(fd_img, "1022", sizeof(int));
    write(fd_img, "1", sizeof(short));
    write(fd_img, "24", sizeof(short));
    write(fd_img, "0", sizeof(unsigned int));
    write(fd_img, "3066000", sizeof(unsigned int));
    write(fd_img, "0", sizeof(int));
    write(fd_img, "0", sizeof(int));
    write(fd_img, "0", sizeof(unsigned int));
    write(fd_img, "0", sizeof(unsigned int));
}

char   *init_image(t_header *fh, int *fd)
{
    char   *img;

    img = (char *)malloc(sizeof(char) * fh->bf_size);
    if (*fd < 0)
        exit(0);
    return (img);
}

char   *get_bmp_img(int *height, int *width)
{
    t_header    fh;
    int         fd_img;
    char        *ret;

    fd_img = open("./my_micky.bmp", O_RDONLY);
    reader(&fh, fd_img);
    ret = init_image(&fh, &fd_img);
    read(fd_img, ret, fh.bf_size);
    *height = fh.height;
    *width = fh.width;

    return (ret);
}

char   *write_image(int r, int g, int b)
{
    static int      i;
    static int      fd = -1;
    t_header        fh;
    char   *img;

    //if (fd <= 0)
        img = init_image(&fh, &fd);
    //else
    {
        img[i] = r;
        img[i + 1] = g;
        img[i + 2] = b;
    }
    update_i(&i, fh);
    if (i >= (int)fh.bf_size)
    {
        write(fd, img, fh.bf_size);
        exit(0);
    }
    return img;
}

void            update_i(int *i, t_header fh)
{
    if (*i < 0)
        *i = fh.image_data_offset;
    *i += 3;
}